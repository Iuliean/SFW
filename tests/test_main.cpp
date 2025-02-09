#define PY_SSIZE_T_CLEAN
#include <gtest/gtest.h>
#include <Python.h>
#include <iostream>

#include "python_utils.hpp"
#include "TSocket.hpp"

int main(int argc, char** argv)
{
    Py_Initialize();
    try
    {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();        
        
    }
    catch(const std::exception& ex)
    {
        Py_FinalizeEx();
        std::cerr << ex.what();
        return -1;
    }
    Py_FinalizeEx();
    return 0;
}
