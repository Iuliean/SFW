#include <stdexcept>
#include <string>
#include <winsock2.h>

#include "platform/win32/WinSockInstance.hpp"

namespace iu
{
    WinSockInstance::WinSockInstance(std::uint8_t majVer, std::uint8_t minVer)
    {
        int result = WSAStartup(MAKEWORD(majVer, minVer), &m_wsaData);
        if (result != 0)
        {
            WSACleanup();
            throw std::runtime_error("Failed to init WinSock instance WSAStartup()=" + std::to_string(result));
        }
    }

    WinSockInstance::~WinSockInstance()
    {
        WSACleanup();
    }
}