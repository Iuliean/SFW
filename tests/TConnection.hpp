#ifndef TCONNECTION_HPP
#define TCONNECTION_HPP
#include <algorithm>
#include <bit>
#include <chrono>
#include <cstdint>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string_view>
#include <SFW/Socket.h>
#include <SFW/Connection.h>
#include <random>
#include <thread>
#include "python_utils.hpp"

struct demo_struct
{
    int a;
    float b;
    long c;
    std::array<int, 3> arr;
};


template<>
struct iu::Serializer<demo_struct>
{
    void Serialize(std::vector<uint8_t>& data, const demo_struct& toSerialzie) const
    {
        auto d = std::bit_cast<std::array<uint8_t, sizeof(demo_struct)>>(toSerialzie);
        data.assign(d.begin(), d.end());
    }

    size_t GetSize(const demo_struct& toSize)const
    {
        return sizeof(demo_struct);
    }
};

namespace sfw_test
{

    constexpr std::string_view  HOSTADDR = "127.0.0.1";
    constexpr uint16_t HOSTPORT = 1234;




    inline bool compare_buffers(std::span<const uint8_t> a, std::span<const uint8_t> b)
    {
        if (a.size() != b.size())
        {
            return false;
        }

        for(size_t idx = 0; idx < a.size(); idx++)
        {
            if (a[idx] != b[idx])
                return false;
        }

        return true;
    }

    class TConnection : public testing::Test
    {
    protected:
        TConnection()
            : m_python_test_module(py_object::load_module("TConnection")),
              m_client_test_instance(),
              m_server_socket()
        {
            m_server_socket.Listen(HOSTADDR.data(), HOSTPORT, 2);

            std::thread([this]()
            {
                if (m_server_socket.Poll(10000))
                    m_client_connection.emplace(m_server_socket.Accept());
            }).detach();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            m_client_test_instance = m_python_test_module.get_attribute("TConnection")->call<py_object>(HOSTADDR.data(), HOSTPORT);


            m_script_set_up = m_client_test_instance.get_attribute("SetUp").value();
            m_script_tear_down = m_client_test_instance.get_attribute("TearDown").value();
            m_script_echo = m_client_test_instance.get_attribute("Echo").value();
        }

        ~TConnection() override = default;

        void SetUp() override
        {
            m_script_set_up();
        }

        void TearDown() override
        {
            m_script_tear_down();
        }

        py_object m_python_test_module;
        py_object m_client_test_instance;

        py_object m_script_set_up;
        py_object m_script_tear_down;
        py_object m_script_echo;

        iu::Socket m_server_socket;
        std::optional<iu::Connection> m_client_connection;
    };

    TEST_F(TConnection, BasicDataTransimision)
    {
        std::array<uint8_t, 100> recieve_buff;
        size_t recieved_amount;

        uint8_t carray[100];
        std::array<uint8_t, 100> stdarr;
        std::vector<uint8_t> stdvec;
        uint8_t* ptrtodata = carray;

        std::random_device rnd;
        std::uniform_int_distribution dist(0,256);

        for ( auto& value : carray)
        {
            value = dist(rnd);
        }

        for (auto& value : stdarr)
        {
            value = dist(rnd);
        }

        stdvec.resize((dist(rnd) % 100) + 1);
        std::generate(stdvec.begin(), stdvec.end(), [&](){ return dist(rnd); });

        m_client_connection->Send(carray);
        m_script_echo();
        recieved_amount = m_client_connection->Receive(recieve_buff);

        EXPECT_TRUE(recieved_amount == sizeof(carray));
        EXPECT_TRUE(compare_buffers(recieve_buff, carray));

        m_client_connection->Send(stdarr);
        m_script_echo();
        recieved_amount = m_client_connection->Receive(recieve_buff);

        EXPECT_TRUE(recieved_amount == stdarr.size());
        EXPECT_TRUE(compare_buffers(recieve_buff, stdarr));

        m_client_connection->Send(stdvec);
        m_script_echo();
        recieved_amount = m_client_connection->Receive(recieve_buff);

        EXPECT_TRUE(recieved_amount == stdvec.size());
        EXPECT_TRUE(compare_buffers({recieve_buff.data(), recieved_amount}, stdvec));

        m_client_connection->Send({ptrtodata, sizeof(carray)});
        m_script_echo();
        recieved_amount = m_client_connection->Receive(recieve_buff);

        EXPECT_TRUE(recieved_amount == sizeof(carray));
        EXPECT_TRUE(compare_buffers(recieve_buff, {ptrtodata, sizeof(carray)}));

    }

    TEST_F(TConnection, Serialization)
    {
        std::random_device rnd;
        std::uniform_int_distribution dist(0, 10000);
        std::uniform_real_distribution<float> real_dist(0, 10000);
        std::uniform_int_distribution<long> long_dist(0, 10000);

        auto generate_random_demo = [&]() -> demo_struct {
            return {
                .a = dist(rnd),
                .b = real_dist(rnd),
                .c = long_dist(rnd),
                .arr = { dist(rnd), dist(rnd), dist(rnd)}
            };
        };

        std::vector<uint8_t> receive_buff;
        const demo_struct test1 = generate_random_demo();
        std::vector<demo_struct> test2;
        const size_t count_of_structs = dist(rnd);

        for (size_t i = 0; i < count_of_structs; i++)
        {
            test2.emplace_back(generate_random_demo());
        }
        receive_buff.resize(count_of_structs);

        size_t receive_amount;
        m_client_connection->Send(test1);
        m_script_echo();
        receive_amount = m_client_connection->Receive(receive_buff);

        EXPECT_TRUE(receive_amount = sizeof(test1));
        EXPECT_TRUE(compare_buffers({receive_buff.data(), receive_amount}, std::bit_cast<std::array<uint8_t, sizeof(test1)>>(test1)));
    }
}

#endif //TCONNECTION_HPP