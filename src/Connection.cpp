#include <cstddef>

#include "Connection.h"
#include "LoggerManager.h"
#include "utils.h"

namespace iu
{
    Connection::Connection(int32_t descriptor, sockaddr_in& details)
        : m_logger(LoggerManager::GetLogger("Connection"))
    {
        m_descriptor = std::make_shared<SocketDescriptor>(descriptor);
        ParseSockDetails(details);

    }
    
    size_t Connection::Send(const std::vector<uint8_t>& data, size_t count)const
    {
        ASSERT((int)*m_descriptor > -1, "Socket not valid")
        ssize_t sent = send((int)*m_descriptor, data.data(), count, 0);
        if(sent == -1)
        {
            m_logger.error("Failed to send data:{}", utils::getErrorFromErrno());
            exit(1);
        }

        return sent;
    }
    //private

    void Connection::ParseSockDetails(sockaddr_in& details)
    {
        //not thread safe
        char* asString = inet_ntoa(details.sin_addr);

        m_address = asString;
        m_port = ntohs(details.sin_port);
    }
}