#include <array>
#include <bits/stdint-uintn.h>
#include <vector>

#include "ServerConnectionHandler.h"

namespace iu
{
    void DefaultConnectionHandler::OnConnected(Connection &connection)
    {
        std::cout << "Connected: " << connection.GetAdress() 
                    << ":"<< connection.GetPort() << '\n'; 
    }

    void DefaultConnectionHandler::HandleConnection(Connection& connection)
    {
        std::array<uint8_t, 512> receivedData;
        std::vector<uint8_t> toSend;
        toSend.resize(512);
        m_stop = false;
        size_t amountReceived = 0;
        while(!m_stop.load())
        {
            amountReceived = connection.Receive(receivedData);

            if(amountReceived == 0)
                return;

            toSend.assign(receivedData.begin(), receivedData.end());

            connection.Send(toSend, amountReceived);
        }
    }

    void DefaultConnectionHandler::Stop()
    {
        m_stop = true;
    }
}