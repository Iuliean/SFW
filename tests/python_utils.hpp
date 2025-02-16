#ifndef PYTHON_UTILS_HPP
#define PYTHON_UTILS_HPP
#include <longobject.h>
#include <string_view>
#include <iostream>
#include <stdexcept>
#include <optional>

#include <Python.h>
#include <type_traits>

namespace sfw_test
{
    class VariableString
    {
    public:
        VariableString() = default;
    
        template<typename T>
        void Append();
    
        template<typename T>
        void Append() requires (std::is_arithmetic_v<
                                        std::remove_cv_t<
                                            std::remove_pointer_t<std::remove_reference_t<T>>
                                        >
                                > && !std::same_as<T, const char*>)
        {
            str += "i";
        }
    
        const std::string& string() const noexcept { return str; }
    private:
        std::string str;
    };
    
    template<>
    void VariableString::Append<const char*>()
    {
        str += "s";
    }
    
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
        PythonClassInstance() noexcept : m_class_obj() {}

        PythonClassInstance(PythonObject obj)
            :m_class_obj(obj)
        {
        }

        template<typename ...Args>
        std::optional<long> CallMethod(std::string_view method, Args&&... args)const
        {
            VariableString str;
            (str.Append<std::decay_t<Args>>(), ...);
            std::cout << str.string() << "\n";
            const char* format = str.string().c_str();
            PythonObject result = PyObject_CallMethod(m_class_obj, method.data(), format ? format : nullptr, args...);

            if(!result)
            {
                PyErr_Print();
                throw std::runtime_error("Failed to call method");
            }

            return PyLong_Check(result) ? PyLong_AsLong(result) : std::optional<long>{};
        }
    private:
        PythonObject m_class_obj;

    };

    class PythonModule
    {
    public:
        PythonModule() noexcept : m_object() {}

        PythonModule(std::string_view file_name)
        {
            PythonObject name = PyUnicode_DecodeFSDefault(file_name.data());
            if (!name)
            {
                std::cerr << "Failed on PyUnicode_DecodeFSDefault\n";
                PyErr_Print();
                throw std::runtime_error("Failed to decode string");
            }

            m_object = PyImport_Import(name);

            if (!m_object)
            {
                std::cerr << "Failed to PyImport_Import\n";
                PyErr_Print();
                throw std::runtime_error("Failed to import Python module");
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
