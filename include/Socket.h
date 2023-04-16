#ifndef SOCKET_H
#define SOCKET_H
#include <bits/c++config.h>
#include <memory>
#include <netinet/in.h>

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
        
        static Connection Connect(const std::string& address, uint16_t port);
    private:
        void Bind(const std::string& address, uint16_t port);
        std::shared_ptr<SocketDescriptor> m_descriptor;
        sockaddr_in m_address;
        socklen_t m_addressLength;
    };
}


#endif //SOCKET_H