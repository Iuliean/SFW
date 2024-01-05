#ifndef SERVER_H
#define SERVER_H
#include <atomic>
#include <bits/stdint-uintn.h>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <set>
#include <mutex>
#include <type_traits>

#include "Logger.h"
#include "LoggerManager.h"
#include "ServerConnectionHandler.h"
#include "Connection.h"
#include "ThreadPool.h"
#include "Socket.h"

namespace iu
{
    /**
     * @brief Base Server class from which AggregateServer and DistributedServer inherit
     * 
     */
    class Server
    {
    public:

        /**
         * @brief Construct a new Server object
         * 
         * @param address address using dot notation. Eaxmple: 127.0.0.1
         * @param port port on which the server binds to.
         * @param maxConnections specifies the maximum amount of connections the server is allowed to handle (default: 40)
         */
        Server(const std::string& address, uint16_t port, size_t maxConnections = 40, const std::string& name= "Default");
        
        Server(const Server&) = delete;
        Server(Server&&) = delete;
        
        Server& operator=(const Server&)=delete;
        Server& operator=(Server&&)=delete;
        
        virtual ~Server();

        void Run();
        virtual void Stop() = 0;
    private:
        virtual void Execute(Connection&&) = 0;
    protected:
        Logger m_logger;
        size_t m_maxConnections;
        std::mutex m_mutexMaxConnections;
        std::condition_variable m_cvMaxConnection;
        std::atomic_bool m_stop;

    private:
        uint16_t m_port;
        std::string m_address;
        Socket m_socket;
        ThreadPool m_threadPool;
    };

    /**
     * @brief Creates a server object that uses the same(shared) Handler for all incoming connections 
     * 
     * @tparam HandlerT Object type used to handle the connection (must inherit from ServerConnectionHandler)
     */
    template<std::derived_from<ServerConnectionHandler> HandlerT = DefaultConnectionHandler> 
    class AggregateServer : public Server
    {
    public:
        AggregateServer(const std::string address, uint16_t port, size_t maxConnections = 40, const std::string& name = "Default")
            : Server(address, port, maxConnections, name)
        {
            m_handler = std::make_unique<HandlerT>();
        }
        ~AggregateServer()override = default;

        void Stop()override
        {
            m_stop = true;
            m_handler->Stop();
        }
    private:
        void Execute(Connection&& connection)override
        {
            m_handler->OnConnected(connection);
            m_handler->HandleConnection(connection);
            {
                std::lock_guard l(m_mutexMaxConnections);
                m_maxConnections++;
                m_cvMaxConnection.notify_all();
            }
        }
    private:
        std::unique_ptr<ServerConnectionHandler> m_handler;
    };
    
    /**
     * @brief Creates a server object where each connection gets a separate instance of the handler
     * 
     * @tparam HandlerT Object type used to handle the connection (must inherit from ServerConnectionHandler) 
     */
    template<std::derived_from<ServerConnectionHandler> HandlerT = DefaultConnectionHandler>
    class DistributedServer : public Server
    {
    public:
        DistributedServer(const std::string& address, uint16_t port, size_t maxConnections = 40, const std::string& name = "Default")
            :Server(address, port, maxConnections, name)
        {

        }
        ~DistributedServer()override = default;
    
        void Stop()override
        {
            std::lock_guard l(m_handlersMutex);
            m_stop = true;
            for(auto& handler : m_handlers)
                handler->Stop();
        }
    private:
        void Execute(Connection&& connection)override
        {
            const HandlerSet::iterator handler = m_handlers.insert(std::make_shared<HandlerT>()).first;
            (*handler)->OnConnected(connection);
            (*handler)->HandleConnection(connection);
            {
                std::scoped_lock l(m_mutexMaxConnections, m_handlersMutex);
                m_maxConnections++;
                m_handlers.erase(handler);
                m_cvMaxConnection.notify_all();
            }
        }
    private:
        using HandlerSet = std::set<std::shared_ptr<ServerConnectionHandler>>;
        HandlerSet m_handlers;
        std::mutex m_handlersMutex;
    };

}
#endif //SERVER_H