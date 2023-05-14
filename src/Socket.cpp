#include <arpa/inet.h>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Socket.h"
#include "utils.h"

namespace iu
{
    static void addressToSockaddr(sockaddr_in* out, const std::string& address, uint16_t port)
    {
        out->sin_family = AF_INET;
        out->sin_port = htons(port);
        if(inet_aton(address.c_str(), &out->sin_addr) == 0) 
        {
            std::cerr << "Address not vaild\n";
            exit(1);
        }
    }

    Socket::Socket()
    {
        m_descriptor  = std::make_shared<SocketDescriptor>();
        *m_descriptor = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); 
        m_epoll = epoll_create1(0);

        if(m_epoll == -1)
        {
            std::cerr << "Failed to create epoll instance: " << utils::getErrorFromErrno() << '\n';
            exit(1);
        }    

        m_epollEvent.events = EPOLLIN;
        m_epollEvent.data.fd = (int)*m_descriptor;

        if(epoll_ctl(m_epoll, EPOLL_CTL_ADD, (int)*m_descriptor, &m_epollEvent) == -1)
        {
            std::cerr << "Failed to add epoll event: " << utils::getErrorFromErrno() << '\n';
            exit(1);
        }
    }

    void Socket::Listen(const std::string& address, uint16_t port, int32_t queueSize)
    {
        Bind(address, port);
        if(listen((int)*m_descriptor, queueSize) == -1)
        {
            std::cerr << "Failed to listen reason: " << utils::getErrorFromErrno() << '\n';
        }
    }

    Connection Socket::Accept()
    {
        sockaddr_in connDetails;
        socklen_t connLen = sizeof(connDetails);
        int32_t connection = accept((int)*m_descriptor, (sockaddr*)&connDetails, &connLen);
        
        if( connection == -1)
        {
            std::cerr << "Failed to accept: " << utils::getErrorFromErrno() << '\n';
            exit(1);
        }
        return {connection, connDetails};
    }

    bool Socket::Poll(int timeout)
    {
        static constexpr int MAXEVENTS = 10;
        epoll_event events[MAXEVENTS]; 
        int numberOfFileDescriptors = epoll_wait(m_epoll, events, MAXEVENTS, timeout);
        
        if(numberOfFileDescriptors == -1 )
        {
            std::cerr << "epoll_wait failed: " << utils::getErrorFromErrno() << '\n';
            exit(1);
        }
        else
            return numberOfFileDescriptors;
    }

    Connection Socket::Connect(const std::string& address, uint16_t port)
    {
        int32_t SD = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in sockAddr;
        addressToSockaddr(&sockAddr, address, port);

        if(connect(SD, (sockaddr*)&sockAddr, sizeof(sockAddr)) == -1)
        {
            std::cerr << "Failed to connect:" << utils::getErrorFromErrno() << '\n';
            exit(1); 
        }

        return {SD, sockAddr};
    }
    
    void Socket::Bind(const std::string& address, uint16_t port)
    {
        addressToSockaddr(&m_address, address, port);

        if(bind((int)*m_descriptor, (sockaddr*)& m_address, sizeof(m_address)) == -1)
        {
            std::cerr << "Failed to bind descriptor: " << utils::getErrorFromErrno() << '\n';
            exit(1);
        }
    }


}