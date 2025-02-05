#include <cstdint>
#include <winsock2.h>

#include "platform/win32/SocketDescriptor.h"
#include "LoggerManager.h"
#include "utils.h"

namespace iu
{
    constexpr auto DOM = "SocketDescriptor";

    SocketDescriptor::SocketDescriptor() noexcept
        : m_descriptor(INVALID_SOCKET){}
    
    SocketDescriptor::SocketDescriptor(SOCKET descriptor) noexcept
        : m_descriptor(descriptor){}
    
    SocketDescriptor::SocketDescriptor(SocketDescriptor&& other) noexcept
    {
        m_descriptor = other.m_descriptor;
        other.m_descriptor = INVALID_SOCKET;
    }

    SocketDescriptor& SocketDescriptor::operator=(SocketDescriptor&& other)noexcept
    {
        m_descriptor = other.m_descriptor;
        other.m_descriptor = INVALID_SOCKET;
        return *this;
    }

    SocketDescriptor::~SocketDescriptor()
    {
        if (m_descriptor != INVALID_SOCKET)
        {
            if (closesocket(m_descriptor) == SOCKET_ERROR)
            {
                SFW_LOG_ERROR(DOM, "Failed to close socket {}: {}", m_descriptor, WSAGetLastError());
            } 
        }
    }

    bool SocketDescriptor::operator==(const SocketDescriptor& descriptor) const noexcept
    {
        return m_descriptor == descriptor.m_descriptor;
    }

    bool SocketDescriptor::operator==(SOCKET descriptor) const noexcept
    {
        return m_descriptor == descriptor;
    } 

    SocketDescriptor::operator bool()const noexcept
    {
        return m_descriptor != INVALID_SOCKET;
    }

    SocketDescriptor::operator SOCKET() noexcept
    {
        return m_descriptor;
    }

    size_t SocketDescriptor::Receive(std::span<uint8_t> buffer) const
    {
        ASSERT(*this, "Socket not Bound");
        int out = recv(m_descriptor, (char*)buffer.data(), buffer.size(), 0); 
        if (out == SOCKET_ERROR)
        {
            SFW_LOG_ERROR(DOM, "Failed to receive data on descriptor: {}", WSAGetLastError());
            return 0;
        }
        return out;
    }

    size_t SocketDescriptor::Send(const std::span<const uint8_t> buffer) const
    {    
        ASSERT(*this, "Socket not Bound");        
        int out = send(m_descriptor, (char*)buffer.data(), buffer.size(), 0);
        if (out == SOCKET_ERROR)
        {
            SFW_LOG_ERROR(DOM, "Failed to transmit data on descriptor: {}", WSAGetLastError());
            return 0;
        }
        return out;
    }
}