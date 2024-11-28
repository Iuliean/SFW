#ifndef SOCKET_PLATFORM_H
#define SOCKET_PLATFORM_H
#include <memory>
#include <netinet/in.h>
#include <sys/epoll.h>

#include "Logger.h"
#include "Connection.h"
#include "SocketDescriptor.h"

namespace iu
{
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
        sockaddr_in m_address;
        socklen_t m_addressLength;
        epoll_inst m_epoll;
        epoll_event m_epollEvent;

        Logger m_logger;
    };
}


#endif //SOCKET_PLATFORM_H