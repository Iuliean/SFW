#ifndef SOCKETDESCRIPTOR_H
#define SOCKETDESCRIPTOR_H
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
        static std::shared_ptr<spdlog::logger> s_logger;
    };
}
#endif //SOCKETDESCRIPTOR_H