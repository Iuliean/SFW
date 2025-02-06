#ifndef SOCKET_DESCRIPTOR_PLATFORM_H
#define SOCKET_DESCRIPTOR_PLATFORM_H
#include <cstdint>
#include <span>
namespace iu
{
    class SocketDescriptor
    {
    public:
        SocketDescriptor()noexcept;
        SocketDescriptor(int32_t descriptor)noexcept;

        SocketDescriptor(SocketDescriptor& sd)noexcept = delete;
        SocketDescriptor& operator=(const SocketDescriptor&)noexcept = delete;
        
        SocketDescriptor(SocketDescriptor&& other)noexcept;
        SocketDescriptor& operator=(SocketDescriptor&& other)noexcept;

        ~SocketDescriptor();
        bool operator==(const SocketDescriptor& descriptor)const noexcept;
        bool operator==(int32_t descriptor)const noexcept;
        operator bool()const noexcept; 
        explicit operator int()const noexcept;

        size_t Send(std::span<const std::uint8_t> data)const;
        size_t Receive(std::span<std::uint8_t> data)const;
    private:
        int32_t m_socketDescriptor;
    };
}
#endif //SOCKET_DESCRIPTOR_PLATFORM_H