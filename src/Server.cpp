#include "Server.h"
#include <bits/stdint-uintn.h>
#include <mutex>

namespace iu
{
    Server::Server(const std::string& address, uint16_t port, size_t maxConnections)
        : m_maxConnections(maxConnections),  
        m_stop(false),
        m_port(port),
        m_address(address),
        m_socket(),
        m_threadPool(30)
    {
    }

    Server::~Server()
    {
        m_stop = true;
    }

    void Server::Run()
    {
        m_socket.Listen(m_address, m_port, m_maxConnections);
        Callback executor(&Server::Execute, this);
        while(!m_stop.load())
        {
            std::unique_lock ul(m_mutexMaxConnections);
            while(m_maxConnections == 0)
                m_cvMaxConnection.wait(ul, [this](){
                    return m_maxConnections > 0 || m_stop;
                });

            if(m_stop.load())
                return;
            
            Connection c = m_socket.Accept();
            m_threadPool.AddTask(executor, std::move(c));
            m_maxConnections--;
        }
    }
}