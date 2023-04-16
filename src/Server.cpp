#include "Server.h"
#include <bits/stdint-uintn.h>

namespace iu
{
    Server::Server(const std::string& address, uint16_t port, size_t maxConnections)
        : m_stop(false), 
        m_port(port),
        m_address(address),
        m_socket(),
        m_maxConnections(maxConnections),
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
            //to enforce max num of connections
            Connection c = m_socket.Accept();
            m_threadPool.AddTask(executor, std::move(c));
        }
    }
}