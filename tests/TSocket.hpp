#ifndef TSOCKET_HPP
#define TSOCKET_HPP
#include <chrono>
#include <gtest/gtest.h>
#include <thread>
#include <random>

#include "python_utils.hpp"
#include "Socket.h"
#include "Connection.h"

namespace sfw_test
{
    class TSocket : public testing::Test
    {
    protected:
        TSocket()
        {
            try
            {
                m_python_test_module = py_object::load_module("TSocket");
                m_client_test_instance = m_python_test_module.get_attribute("TSocket")->call<py_object>();

                m_script_set_up = m_client_test_instance.get_attribute("SetUp").value();
                m_script_tear_down = m_client_test_instance.get_attribute("TearDown").value();

                m_script_listen_accept = m_client_test_instance.get_attribute("ListenAccept").value();
                m_script_poll_connect = m_client_test_instance.get_attribute("PollConnect").value();
            }
            catch(const std::exception& ex)
            {
                std::cerr << "Failed to start Tsocket: " << ex.what() << "\n";
                throw;
            }
        }
        ~TSocket() override = default;

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

        py_object m_script_listen_accept;
        py_object m_script_poll_connect;
    };

    TEST_F(TSocket, PollTest)
    {
        using namespace std::chrono_literals;
        const std::string address = "127.0.0.1";
        const int port = 12345;
        std::atomic_bool stop = false;
        std::atomic_bool poll_result = false;

        /*
        * Simple case of connecting to the socket and checking that the Poll method returns true 
        */
        {
            iu::Socket s;
            s.Listen(address, port, 0);

            std::thread t ([&s, &stop, &poll_result]
            {
                while(!stop)
                {
                    if (s.Poll() == true)
                        poll_result = true;
                }
            });

            m_script_poll_connect(address.c_str(), port);
            stop = true;
            t.join();

            EXPECT_TRUE(poll_result);
        }

        /*
        * Continuously polling to check that if nobody tries to connect the result is always false
        */
        {
            iu::Socket s;
            s.Listen(address, port, 0);
            stop = false;

            std::thread t([&]()
            {
                while(stop)
                {
                    EXPECT_FALSE(s.Poll());
                }
            });

            // Allow some time to pass
            std::this_thread::sleep_for(10s);
            stop = true;
            t.join();
        }

        //The next few tests check the timout of the Poll method
        {
            iu::Socket s;
            s.Listen(address, port, 0);

            std::thread t([&]()
            {
                // TODO: change the interface to accept std::duration for now it is milliseconds
                EXPECT_TRUE(s.Poll(2000));
            });

            std::this_thread::sleep_for(1s);
            m_script_poll_connect(address.c_str(), port);
            t.join();
        }

        {
            iu::Socket s;
            s.Listen(address, port, 0);

            std::thread t([&]()
            {
                // TODO: change the interface to accept std::duration for now it is milliseconds
                EXPECT_FALSE(s.Poll(1000));
            });

            std::this_thread::sleep_for(2s);
            m_script_poll_connect(address.c_str(), port);
            t.join();
        }
    }

    TEST_F(TSocket, ListenAccept)
    {
        const std::string address = "127.0.0.1";
        const int port = 12345;

        std::atomic_bool has_accepted = false;
        std::atomic_bool stop = false;
        iu::Socket s;
        s.Listen(address, port, 0);

        std::thread t([this, &s, &has_accepted, &stop](){
            while (!stop)
            {
                if (s.Poll())
                {
                    const auto acc = s.Accept();
                    has_accepted = true;
                    break;
                }
            }
            });

        m_script_listen_accept(address.c_str(), port);
        stop = true;
        t.join();

        EXPECT_TRUE(has_accepted);
    }
}

#endif //TSOCKET_H