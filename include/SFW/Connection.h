#ifndef CONNECTION_H
#define CONNECTION_H
#include <array>
#include <memory>
#include <vector>
#include <cstdint>
#include <string.h>

#include "utils.h"
#include "Logger.h"
#include "Serializer.h"
#include "SocketDescriptor.h"

namespace iu
{
    class SFW_API Connection
    {
    public:
        Connection(std::shared_ptr<SocketDescriptor> descriptor, sockaddr_in& details);

        Connection(const Connection& other) = default;
        Connection& operator=(const Connection& other) = default;

        Connection(Connection&& other) = default;
        Connection& operator=(Connection&& other) = default;

        size_t SendAll(const std::vector<uint8_t>& data)const { return Send(data, data.size()); }

        size_t Send(const std::vector<uint8_t>& data, size_t count)const
        {
            ASSERT(m_descriptor, "Socket not valid");
            return m_descriptor->Send({data.begin(), count});
        }
        
        template<typename T, template<typename> typename SerializerT = Serializer>
        size_t Send(const std::vector<T>& objects) const requires Serializable<T, SerializerT>
        {
            SerializerT<T> s;
            std::vector<uint8_t> send;
            if constexpr(SizeSerializable<T>)
            {
                size_t sumSize = 0;
                for(T& o : objects)
                    sumSize += s.GetSize(o);
                send.reserve(sumSize);
            }
            for(auto& obj : objects)
                s.Serialize(send, obj);

            return SendAll(send);
        }

        template<typename T, template<typename> typename SerializerT = Serializer>
        size_t Send(const T& object)const requires Serializable<T, SerializerT>
        {
            SerializerT<T> s;
            std::vector<uint8_t> send;
            if constexpr(SizeSerializable<T>)
            {
                send.reserve(s.GetSize(object));
            }
            s.Serialize(send, object);
            return SendAll(send);
        }

        template<size_t N = 1024>
        size_t Receive(std::array<uint8_t, N>& data)const
        {
            ASSERT(*m_descriptor, "Socket not valid");
            return m_descriptor->Receive(data);
        }

        size_t Receive(std::vector<uint8_t>& data, size_t count)const
        {
            ASSERT(*m_descriptor, "Socket not valid");
            return m_descriptor->Receive({data.begin(), count});
        }

        std::string GetAdress() const { return m_address; }
        
        uint16_t GetPort() const { return m_port; }
    private:
        void ParseSockDetails(sockaddr_in& details);
    private:
        Logger m_logger;
        std::string m_address;
        std::uint16_t m_port;
        std::shared_ptr<SocketDescriptor> m_descriptor;
    };

}

#endif //CONNECTION_H