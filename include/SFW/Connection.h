#ifndef CONNECTION_H
#define CONNECTION_H
#include <array>
#include <memory>
#include <span>
#include <vector>
#include <cstdint>
#include <string.h>

#include "Socket.h"
#include "utils.h"
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

        size_t Send(std::span<const std::uint8_t> data) const { return Send(data, data.size()); }

        size_t Send(std::span<const std::uint8_t> data, size_t count)const
        {
            ASSERT(m_descriptor, "Socket not valid");
            return m_descriptor->Send(data.subspan(0, count));
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

            return Send(std::span<const std::uint8_t>{send.data(), send.size()});
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
            return Send(std::span<const std::uint8_t>{send.data(), send.size()});
        }

        size_t Receive(std::span<std::uint8_t> data)const
        {
            ASSERT(*m_descriptor, "Socket not valid");
            return m_descriptor->Receive(data);
        }

        std::string GetAdress() const { return m_address; }
        
        uint16_t GetPort() const { return m_port; }
    private:
        void ParseSockDetails(sockaddr_in& details);
    private:
        std::string m_address;
        std::uint16_t m_port;
        std::shared_ptr<SocketDescriptor> m_descriptor;
    };

}

#endif //CONNECTION_H