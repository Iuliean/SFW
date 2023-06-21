#ifndef SERVER_CONNECTION_HANDLER_H
#define SERVER_CONNECTION_HANDLER_H
#include <atomic>
#include <memory>
#include <thread>
#include <chrono>

#include "Connection.h"
#include "spdlog/logger.h"
namespace iu
{
    class ServerConnectionHandler
    {
    public:
        ServerConnectionHandler() = default;
        virtual ~ServerConnectionHandler() = default;
        virtual void HandleConnection(Connection& connection) = 0;
        virtual void OnConnected(Connection& connection) = 0;
        virtual void Stop() = 0;
    };

    class DefaultConnectionHandler : public ServerConnectionHandler
    {
    public:
        DefaultConnectionHandler();
        ~DefaultConnectionHandler()override = default;
        void OnConnected(Connection& connection)override;
        void HandleConnection(Connection& connection)override;
        void Stop()override;
    private:
        std::atomic_bool m_stop;
        std::shared_ptr<spdlog::logger> m_logger;
    };
}

#endif //SERVER_CONNECTION_HANDLER_H