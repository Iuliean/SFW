#include <array>
#include <vector>

#include "LoggerManager.h"
#include "ServerConnectionHandler.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace iu
{
    DefaultConnectionHandler::DefaultConnectionHandler()
        : ServerConnectionHandler(),
        m_logger(LoggerManager::GetLogger("DefaultConectionHandler"))
    {
    }

    void DefaultConnectionHandler::OnConnected(Connection &connection)
    {
        m_logger.info("Connected to {}:{}", connection.GetAdress(), connection.GetPort());
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