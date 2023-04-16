#ifndef SERVER_CONNECTION_HANDLER_H
#define SERVER_CONNECTION_HANDLER_H
#include <thread>
#include <chrono>

#include "Connection.h"
namespace iu
{
    class ServerConnectionHandler
    {
    public:
        ServerConnectionHandler() = default;
        virtual ~ServerConnectionHandler() = default;
        virtual void HandleConnection(Connection& connection) = 0;
        virtual void OnConnected(Connection& connection) = 0;
    };

    class DefaultConnectionHandler : public ServerConnectionHandler
    {
    public:
        DefaultConnectionHandler() = default;
        ~DefaultConnectionHandler()override = default;
        void OnConnected(Connection& connection)override;
        void HandleConnection(Connection& connection)override;
    };
}

#endif //SERVER_CONNECTION_HANDLER_H