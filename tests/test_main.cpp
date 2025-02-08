#define PY_SSIZE_T_CLEAN
#include <gtest/gtest.h>
#include <Python.h>
#include <Windows.h>
#include <winbase.h>

#include "python_utils.hpp"


int main(int argc, char** argv)
{
    Py_Initialize();
    try
    {

        
        sfw_test::PythonModule module("hello");
        module.GetFunction("test_func")();
        const auto cls = module.GetClassInstance("Check");
        cls.CallMethod("method");

        testing::InitGoogleTest(&argc, argv);
        const auto result = RUN_ALL_TESTS();
        
        
        return result;
    }
    catch(const std::exception& ex)
    {
        Py_FinalizeEx();
        std::cerr << ex.what();
        return -1;
    }
    Py_FinalizeEx();

}
