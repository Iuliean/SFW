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
        
        size_t amountReceived = 0;
        while(true)
        {
            amountReceived = connection.Receive(receivedData);

            toSend.assign(receivedData.begin(), receivedData.end());

            connection.Send(toSend, amountReceived);
        }
    }
}