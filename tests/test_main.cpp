#define PY_SSIZE_T_CLEAN
#include <cstddef>
#include <gtest/gtest.h>
#include <Python.h>
#include <pythonrun.h>


int main(int argc, char** argv)
{
    Py_Initialize();

    //PyRun_SimpleString("print(\"hello from python\")");
    const auto fd= fopen("test.py","rb");
    PyRun_SimpleFile(fd, "test.py");
    testing::InitGoogleTest(&argc, argv);
    const auto result = RUN_ALL_TESTS();

    Py_FinalizeEx();

    return result;
}