#ifndef SERVER_H
#define SERVER_H
#include <atomic>
#include <bits/stdint-uintn.h>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>

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
        Server(const std::string& address, uint16_t port, size_t maxConnections = 40);
        
        Server(const Server&) = delete;
        Server(Server&&) = delete;
        
        Server& operator=(const Server&)=delete;
        Server& operator=(Server&&)=delete;
        
        virtual ~Server();

        void Run();
    private:
        virtual void Execute(Connection&&) = 0;
    private:
        std::atomic_bool m_stop;
        uint16_t m_port;
        std::string m_address;
        Socket m_socket;
        std::atomic_int m_maxConnections;
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
        AggregateServer(const std::string address, uint16_t port, size_t maxConnections = 40)
            : Server(address, port, maxConnections)
        {
            m_handler = std::make_unique<HandlerT>();
        }
        ~AggregateServer()override = default;
    private:
        void Execute(Connection&& connection)override
        {
            m_handler->OnConnected(connection);
            m_handler->HandleConnection(connection);
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
        DistributedServer(const std::string& address, uint16_t port, size_t maxConnections = 40)
            :Server(address, port, maxConnections)
        {
        }
        ~DistributedServer()override = default;
    private:
        void Execute(Connection&& connection)override
        {
            std::unique_ptr<ServerConnectionHandler> handler = std::make_unique<HandlerT>();
            handler->OnConnected(connection);
            handler->HandleConnection(connection);
        }
    };
}

#endif //SERVER_H