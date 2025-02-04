#ifndef SOCKET_DESCRIPTOR_PLATFORM_H
#define SOCKET_DESCRIPTOR_PLATFORM_H
#include <cstdint>
#include <winsock2.h>
#include <span>
#include "utils.h"
namespace iu
{
    class SFW_API SocketDescriptor
    {
    public:
        SocketDescriptor() noexcept;
        SocketDescriptor(SOCKET descriptor) noexcept;

        SocketDescriptor(SocketDescriptor& sd) = delete;
        SocketDescriptor& operator=(const SocketDescriptor&) = delete;

        SocketDescriptor(SocketDescriptor&& other) noexcept;
        SocketDescriptor& operator=(SocketDescriptor&& other)noexcept;
        
        ~SocketDescriptor();
        bool operator==(const SocketDescriptor& descriptor)const noexcept;
        bool operator==(SOCKET descriptor)const noexcept;
        operator bool()const noexcept;
        explicit operator SOCKET()noexcept;

        size_t Send(std::span<const std::uint8_t> data) const;
        size_t Receive(std::span<std::uint8_t> data) const; 
    private:
        SOCKET m_descriptor;
    };
}
#endif //SOCKET_DESCRIPTOR_PLATFORM_H