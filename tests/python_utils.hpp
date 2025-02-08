#ifndef PYTHON_UTILS_HPP
#define PYTHON_UTILS_HPP
#include <string_view>
#include <iostream>
#include <stdexcept>

#include <Python.h>

namespace sfw_test
{
    class PythonObject
    {
    public:
        PythonObject()noexcept
            : m_object(NULL) {}
        
        PythonObject(PyObject* obj)noexcept
            : m_object(obj) {}

        PythonObject(const PythonObject& other)noexcept
            : m_object(other.m_object)
        {
            Py_INCREF(m_object);
        }

        PythonObject& operator=(const PythonObject& other)noexcept
        {
            m_object = other.m_object;
            Py_INCREF(m_object);
            return *this;
        }

        PythonObject(PythonObject&& other)noexcept
            : m_object(other.m_object)
        {
            other.m_object = NULL;
        }

        PythonObject& operator=(PythonObject&& other)noexcept
        {
            m_object = other.m_object;
            other.m_object = NULL;
            return *this;
        }

        PythonObject& operator=(PyObject* obj) noexcept
        {
            m_object = obj;
            return *this;
        }

        operator PyObject*()const noexcept { return m_object; }

        bool operator==(const PythonObject& other) const noexcept { return other.m_object == m_object; }
        bool operator==(const PyObject*const other) const noexcept { return other == m_object; }
        operator bool()const noexcept { return m_object != NULL; }
        ~PythonObject()
        {
            Py_XDECREF(m_object);
        }
    private:
        PyObject* m_object;
    };

    class PythonFunction
    {
    public:
        PythonFunction() = default;
        PythonFunction(PythonObject funcObj) noexcept
            : m_obj(funcObj){}
        
        void operator()()const
        {
            if(PyObject_CallObject(m_obj, NULL) == NULL)
            {
                std::runtime_error("Call failed");
            }
        }

        ~PythonFunction() = default;
    private:
        PythonObject m_obj;    
    };

    class PythonClassInstance
    {
    public:
        PythonClassInstance(PythonObject obj)
            :m_class_obj(obj)
        {
        }

        void CallMethod(std::string_view method)const
        {
            PythonObject result = PyObject_CallMethod(m_class_obj, method.data(), "");
            if(!result)
            {
                PyErr_Print();
                throw std::runtime_error("Failed to call method");
            }
        }
    private:
        PythonObject m_class_obj;
    };

    class PythonModule
    {
    public:
        PythonModule(std::string_view file_name)
        {
            PythonObject name = PyUnicode_DecodeFSDefault(file_name.data());
            if (!name)
            {
                std::cerr << "Failed on PyUnicode_DecodeFSDefault\n";
                PyErr_Print();
                std::terminate();
            }

            m_object = PyImport_Import(name);

            if (!m_object)
            {
                std::cerr << "Failed to PyImport_Import\n";
                PyErr_Print();
                std::terminate();
            }
        }

        PythonFunction GetFunction(std::string_view name)const
        {
            PythonObject obj = PyObject_GetAttrString(m_object, name.data()); 
            if(!obj)
            {
                PyErr_Print();
                throw std::runtime_error("Failed to retrieve object");
            }
            return obj;
        }
        
        PythonClassInstance GetClassInstance(std::string_view name)const
        {
            PythonObject class_instance = PyObject_CallMethod(m_object, name.data(), "");
            if (!class_instance)
            {
                PyErr_Print();
                throw std::runtime_error("Failed to construct class instance");
            }

            return class_instance;
        }

        ~PythonModule() = default;
    private:
        PythonObject m_object;
    };
}

#endif //PYTHON_UTILS_HPP
