#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "SocketDescriptor.h"
#include "platform/posix/SocketDescriptor.h"
#include "utils.h"
#include "LoggerManager.h"
namespace iu
{
    constexpr auto DOM = "SocketDescriptor";
    SocketDescriptor::SocketDescriptor(int32_t descriptor)noexcept
    {
        m_socketDescriptor = descriptor;
    }
    SocketDescriptor::SocketDescriptor()noexcept
        :m_socketDescriptor(-1){}
    
    SocketDescriptor::SocketDescriptor(SocketDescriptor&& other)noexcept
    {
        m_socketDescriptor = other.m_socketDescriptor;
        other.m_socketDescriptor = -1;
    }

    SocketDescriptor& SocketDescriptor::operator=(SocketDescriptor&& other)noexcept
    {
        m_socketDescriptor = other.m_socketDescriptor;
        other.m_socketDescriptor = -1;
        return *this;
    }

    SocketDescriptor::~SocketDescriptor()
    {
        if(m_socketDescriptor > -1)
        {
            if(shutdown(m_socketDescriptor, SHUT_RDWR) == -1)
            {
                SFW_LOG_ERROR(DOM, "Failed to shutdown socket: {}", utils::getErrorFromErrno());
                return;
            }

            if (close(m_socketDescriptor) == -1)
            {
                SFW_LOG_ERROR(DOM, "Failed to close socket: {}", utils::getErrorFromErrno());
                return;
            }
            
        }
    }

    SocketDescriptor::operator bool()const noexcept
    {
        return m_socketDescriptor > -1;
    }

    bool SocketDescriptor::operator==(const SocketDescriptor& descriptor)const noexcept
    {
        return descriptor.m_socketDescriptor == m_socketDescriptor;
    }
    bool SocketDescriptor::operator==(int32_t descriptor)const noexcept
    {
        return m_socketDescriptor == descriptor;
    }

    SocketDescriptor::operator int()const noexcept
    {
        return m_socketDescriptor;
    }

    size_t SocketDescriptor::Send(std::span<const std::uint8_t> data) const
    {
        return send(m_socketDescriptor, (void*)data.data(), data.size(), 0);
    }

    size_t SocketDescriptor::Receive(std::span<std::uint8_t> data)const
    {
        return recv(m_socketDescriptor, (void*)data.data(), data.size(), 0);
    }

}