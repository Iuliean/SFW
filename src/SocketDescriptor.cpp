#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "SocketDescriptor.h"
#include "utils.h"

namespace iu
{
    std::shared_ptr<spdlog::logger> SocketDescriptor::s_logger = spdlog::stdout_color_mt("SocketDescriptor");

    SocketDescriptor::SocketDescriptor(int32_t descriptor)
    {
        m_socketDescriptor = descriptor;
    }
    SocketDescriptor::SocketDescriptor()
        :m_socketDescriptor(-1){}
    
    SocketDescriptor::SocketDescriptor(SocketDescriptor&& other)
    {
        m_socketDescriptor = other.m_socketDescriptor;
        other.m_socketDescriptor = -1;
    }

    SocketDescriptor::~SocketDescriptor()
    {
        if(m_socketDescriptor > -1)
        {
            if(shutdown(m_socketDescriptor, SHUT_RDWR) == -1)
            {
                s_logger->error("Failed to shutdown socket: {}", utils::getErrorFromErrno());
                return;
            }

            if (close(m_socketDescriptor) == -1)
            {
                s_logger->error("Failed to close socket: {}", utils::getErrorFromErrno());
                return;
            }
            
        }
    }

    void SocketDescriptor::operator=(int32_t descriptor)
    {
        ASSERT(m_socketDescriptor < 0, "Socket already assigned");
        m_socketDescriptor = descriptor;
    }

    bool SocketDescriptor::operator==(const SocketDescriptor& descriptor)const
    {
        return descriptor.m_socketDescriptor == m_socketDescriptor;
    }
    bool SocketDescriptor::operator==(int32_t descriptor)const
    {
        return m_socketDescriptor == descriptor;
    }

    SocketDescriptor::operator int()
    {
        return m_socketDescriptor;
    }

}