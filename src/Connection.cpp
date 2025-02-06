#include <cstddef>
#include <cstdint>
#ifndef _WIN32
    #include <arpa/inet.h>
#endif

#include "Connection.h"
#include "LoggerManager.h"
#include "utils.h"

namespace iu
{
    Connection::Connection(std::shared_ptr<SocketDescriptor> descriptor, sockaddr_in& details)
        : m_descriptor(descriptor)
    {
        ParseSockDetails(details);

    }
    //private

    void Connection::ParseSockDetails(sockaddr_in& details)
    {
        //not thread safe
        //this is also deprecated on windows needs chages
        char* asString = inet_ntoa(details.sin_addr);

        m_address = asString;
        m_port = ntohs(details.sin_port);
    }
}