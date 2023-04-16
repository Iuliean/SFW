#ifndef CONNECTION_H
#define CONNECTION_H
#include <array>
#include <bits/stdint-uintn.h>
#include <memory>
#include <netinet/in.h>
#include <vector>
#include <cstdint>
#include <iostream>
#include <string.h>

#include "utils.h"
#include "SocketDescriptor.h"

namespace iu
{
    class Connection
    {
    public:
        Connection(int32_t descriptor, sockaddr_in& details);

        Connection(const Connection& other) = default;
        Connection& operator=(const Connection& other) = default;

        Connection(Connection&& other) = default;
        Connection& operator=(Connection&& other) = default;

        size_t SendAll(const std::vector<uint8_t>& data)const;
        size_t Send(const std::vector<uint8_t>& data, size_t count) const;

        template<size_t N = 1024>
        size_t Receive(std::array<uint8_t, N>& data)const
        {
            ASSERT((int)*m_descriptor > -1, "Socket not valid")
            ssize_t received = recv((int)*m_descriptor, data.data(), N, 0);
            if(received == -1)
            {
                std::cerr << "Failed to receive data: " << strerror(errno);
                exit(1);
            }
            return received;
        }

        std::string GetAdress()const;
        std::uint16_t GetPort()const;
    private:
        void ParseSockDetails(sockaddr_in& details);
    private:
        std::string m_address;
        std::uint16_t m_port;
        std::shared_ptr<SocketDescriptor> m_descriptor;
    };

    inline size_t Connection::SendAll(const std::vector<uint8_t>& data)const
    {
        return Send(data, data.size());
    }

    inline std::string Connection::GetAdress()const
    {
        return m_address;
    }

    inline uint16_t Connection::GetPort()const
    {
        return m_port;
    }
}

#endif //CONNECTION_H