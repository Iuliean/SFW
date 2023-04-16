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
    class Server
    {
    public:
        
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

    //All connections gets the same instance of the Handler
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

    //Each connection has it's own handler for it
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