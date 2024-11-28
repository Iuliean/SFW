#ifndef SOCKET_PLATFORM_H
#define SOCKET_PLATFORM_H

#include <string>

#include "Logger.h"
#include "SocketDescriptor.h"

namespace iu
{
    class Connection;
    class Socket
    {
    public:
        Socket();
        void Listen(const std::string& address, uint16_t port, int32_t queueSize);
        
        [[nodiscard("All connections need to be handled")]]
        Connection Accept();
        bool Poll(int timeout = -1);

        static Connection Connect(const std::string& address, uint16_t port);
    private:
        using epoll_inst = int;
        
        void Bind(const std::string& address, uint16_t port);
        
        std::shared_ptr<SocketDescriptor> m_descriptor;


        Logger m_logger;
    };
}

#endif //SOCKET_PLATFORM_H