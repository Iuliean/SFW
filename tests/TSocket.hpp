#ifndef TSOCKET_HPP
#define TSOCKET_HPP
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

#include "platform/win32/Socket.h"
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
                std::cout << "Lol\n";
                m_python_test_module = PythonModule("TSocket");
                m_client_test_instance = m_python_test_module.GetClassInstance("TSocket");
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
            m_client_test_instance.CallMethod("SetUp");
        }

        void TearDown() override
        {
            m_client_test_instance.CallMethod("TearDown");
        }

        PythonModule m_python_test_module;
        PythonClassInstance m_client_test_instance;
    };
    
    TEST_F(TSocket, ListenAccept)
    {
        iu::Socket s;

        s.Listen("127.0.0.1", 12345, 0);

        std::thread t([this, &s](){ const auto acc = s.Accept(); });
        std::this_thread::sleep_for(std::chrono::seconds(2));
        ASSERT_TRUE(t.joinable());
    }
}
    
#endif //TSOCKET_H