#ifndef SOCKET_PLATFORM_H
#define SOCKET_PLATFORM_H

#include <cstdint>
#include <string>

#include "Logger.h"
#include "SocketDescriptor.h"
#include "WinSockInstance.hpp"

namespace iu
{
    class Connection;
    class SFW_API Socket
    {
    public:
        Socket();
        void Listen(const std::string& address, uint16_t port, int32_t queueSize);
        
        [[nodiscard("All connections need to be handled")]]
        Connection Accept();
        
        void Connect(const std::string& address, uint16_t port);

        bool Poll(int timeout = -1);

    private:
        
        void Bind(const std::string& address, uint16_t port);
        
        WinSockInstance m_wsaInstance;
 
        std::shared_ptr<SocketDescriptor> m_descriptor;

        //to be removed and revamp logging
        Logger m_logger; 
    };
}

#endif //SOCKET_PLATFORM_H