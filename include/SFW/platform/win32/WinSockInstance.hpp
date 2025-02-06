#ifndef WINSOCK_INSTANCE_H
#define WINSOCK_INSTANCE_H

#include <cstdint>
#include <winsock2.h>
#include "utils.h"
#pragma comment(lib, "Ws2_32.lib")

namespace iu
{
    class SFW_API WinSockInstance
    {
    public:
        WinSockInstance(std::uint8_t majVer, std::uint8_t minVer);
        ~WinSockInstance();
    private:
        WSADATA m_wsaData;
    };
}

#endif //WINSOCK_INSTANCE_H