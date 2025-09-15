#include <string_view>
#define PY_SSIZE_T_CLEAN
#include <gtest/gtest.h>
#include <Python.h>

#include "LoggerManager.h"
#include "python_utils.hpp"
//#include "TSocket.hpp"
#include "TConnection.hpp"
int main(int argc, char** argv)
{
    iu::LoggerManager::LogToConsole();
    Py_Initialize();
    try
    {
        testing::InitGoogleTest(&argc, argv);

        return RUN_ALL_TESTS();
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what();
        Py_FinalizeEx();
        return -1;
    }

    Py_FinalizeEx();
    return 0;
}
