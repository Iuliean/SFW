#include <memory>
#include <stdexcept>
#include <string>
#include <ws2tcpip.h>
#include <winsock2.h>

#include "platform/win32/Socket.h"
#include "Connection.h"
#include "LoggerManager.h"
#include "platform/win32/SocketDescriptor.h"

namespace iu
{
    Socket::Socket()
        : m_logger(LoggerManager::GetLogger("Socket")),
          m_wsaInstance(2,2)
    {
        m_descriptor = std::make_shared<SocketDescriptor>();
        *m_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (*m_descriptor == INVALID_SOCKET)
        {
            m_logger.error("Failed to create socket: {}", WSAGetLastError());
            throw std::runtime_error("Failed to create socket");
        }

    }

    void Socket::Listen(const std::string& address, uint16_t port, int32_t queueSize)
    {
        Bind(address, port);
        if (listen((SOCKET)*m_descriptor, queueSize) == SOCKET_ERROR)
        {
            m_logger.error("Failed to listen to socket {}: {}", (SOCKET)*m_descriptor, WSAGetLastError());
        }
    }

    Connection Socket::Accept()
    {
        sockaddr_in connDetails;
        socklen_t connLen = sizeof(connDetails);
        std::shared_ptr connection = std::make_shared<SocketDescriptor>(accept((SOCKET)*m_descriptor, (sockaddr*)&connDetails, &connLen));
        
        if( *connection == INVALID_SOCKET)
        {
            m_logger.error("Failed to accept: {}", WSAGetLastError());
            exit(1);
        }
        return Connection(connection, connDetails);
    }

    bool Socket::Poll(int timeout)
    {
        WSAPOLLFD fds;
        fds.fd = (SOCKET)*m_descriptor;
        fds.events = POLLRDNORM;

        const int result = WSAPoll(&fds, 1, 0);
        std::cout << "Result " << result << "\n";
        if (result == 0 || result == SOCKET_ERROR)
        {
        std::cout << WSAGetLastError() << '\n';
            return false;
        }
        

        return fds.revents & POLLRDNORM;
    }

    void Socket::Connect(const std::string& address, uint16_t port)
    {
        //int32_t SD = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in sockAddr;
        //addressToSockaddr(&sockAddr, address, port);
//
        //if(connect(SD, (sockaddr*)&sockAddr, sizeof(sockAddr)) == -1)
        //{
        //    LoggerManager::GlobalLogger().error("Failed to connect: {}", utils::getErrorFromErrno());
        //    exit(1); 
        //}
//
        //return {SD, sockAddr};
        //return {INVALID_SOCKET,sockAddr};
    }

    void Socket::Bind(const std::string& address, uint16_t port)
    {
        addrinfo hints;
        addrinfo* results;

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &results);

        if (bind((SOCKET)*m_descriptor, results->ai_addr, results->ai_addrlen) == SOCKET_ERROR)
        {
            //m_logger.info("Failed to bind socket", WSAGetLastError());
            std::cout << "FAiled to bind:" << WSAGetLastError();
        }

        freeaddrinfo(results);
    }
}