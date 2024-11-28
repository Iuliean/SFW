#ifndef SOCKET_DESCRIPTOR_PLATFORM_H
#define SOCKET_DESCRIPTOR_PLATFORM_H
#include "spdlog/common.h"
#include "spdlog/logger.h"
#include <cstdint>
#include <memory>

namespace iu
{
    class SocketDescriptor
    {
    public:
        SocketDescriptor();
        SocketDescriptor(int32_t descriptor);
        SocketDescriptor(SocketDescriptor& sd) = delete;
        SocketDescriptor(SocketDescriptor&& other);
        ~SocketDescriptor();
        void operator=(int32_t descriptor);
        bool operator==(const SocketDescriptor& descriptor)const;
        bool operator==(int32_t descriptor)const;
        explicit operator int();
    private:
        int32_t m_socketDescriptor;
    };
}
#endif //SOCKET_DESCRIPTOR_PLATFORM_H