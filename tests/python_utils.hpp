#ifndef PYTHON_UTILS_HPP
#define PYTHON_UTILS_HPP
#include "LoggerManager.h"
#include <Python.h>
#include <abstract.h>
#include <filesystem>
#include <import.h>
#include <longobject.h>
#include <modsupport.h>
#include <object.h>
#include <pyerrors.h>
#include <pyport.h>
#include <concepts>
#include <pythonrun.h>
#include <expected>
#include <pytypedefs.h>
#include <source_location>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <unicodeobject.h>
#include <utility>
namespace sfw_test
{

    class py_exception;
    class py_object;

    enum class py_constant
    {
        none = 0,
        pyfalse,
        pytrue,
        ellipsis,
        not_implemented,
        zero,
        one,
        empty_str,
        empty_bytes,
        empty_tuple
    };

    template<typename T>
    consteval char type_to_format_char() { static_assert(false, "This type is not suppoerted"); return '-';}
    template<>
    consteval char type_to_format_char<const char*>()            { return 's'; }
    template<>
    consteval char type_to_format_char<char*>()                  { return 's'; }
    template<>
    consteval char type_to_format_char<int>()                    { return 'i';}
    template<>
    consteval char type_to_format_char<char>()                   { return 'b';}
    template<>
    consteval char type_to_format_char<short int>()              { return 'h';}
    template<>
    consteval char type_to_format_char<long int>()               { return 'l';}
    template<>
    consteval char type_to_format_char<unsigned char>()          { return 'B';}
    template<>
    consteval char type_to_format_char<unsigned short int>()     { return 'H';}
    template<>
    consteval char type_to_format_char<unsigned int>()           { return 'I';}
    template<>
    consteval char type_to_format_char<unsigned long>()          { return 'k';}
    template<>
    consteval char type_to_format_char<long long>()              { return 'L';}
    template<>
    consteval char type_to_format_char<unsigned long long>()     { return 'K';}
    template<>
    consteval char type_to_format_char<double>()                 { return 'd';}
    template<>
    consteval char type_to_format_char<float>()                  { return 'f';}
    template<>
    consteval char type_to_format_char<py_object>()              { return 'O';}

    template<typename ...Args>
    constexpr std::string build_value_string()
    {
        std::string out;
        if constexpr (sizeof...(Args) > 0)
        {
            ((out += type_to_format_char<Args>()), ...);
        }
        else
        {
            out = "()";
        }
        return out;
    }


    class py_object
    {
    public:
        static py_object get_class_constant(py_constant constant);
        static py_object load_module(const std::string& module_name);
    private:
        static void throw_current_python_exception();
    public:
        py_object()noexcept;
        py_object(PyObject* obj)noexcept;
        py_object(const py_object& other)noexcept;
        py_object& operator=(const py_object& other)noexcept;
        py_object(py_object&& other)noexcept;
        py_object& operator=(py_object&& other)noexcept;
        py_object& operator=(PyObject* obj) noexcept;

        py_object(std::string_view data)noexcept;

        ~py_object();
        operator PyObject*()const noexcept;

        void print(std::filesystem::path file, int flags);

        [[nodiscard]]
        bool has_attribute(const std::string& name) const;

        [[nodiscard]]
        bool is_callable()const noexcept;

        [[nodiscard]]
        std::string_view to_string()const;

        std::optional<py_object> get_attribute(const std::string& name) const;

        template<typename R = void, typename ...Args>
        R call(Args&& ...args)const
        {
            PyObject* tuple_args = Py_BuildValue
            (
                build_value_string<std::decay_t<std::remove_cvref_t<Args>>...>().c_str(),
                args...
            );

            if(!tuple_args)
            {
                throw_current_python_exception();
            }

            PyObject* result = PyObject_Call(m_object, tuple_args, nullptr);

            if (!result)
            {
                throw_current_python_exception();
            }

            //This "works" with PyObject too because its the only element in the class and does not require any special treatment
            if constexpr (!std::is_same_v<void, R>)
            {
                R res;
                PyArg_Parse(result, build_value_string<std::decay_t<std::remove_cvref_t<R>>>().c_str(), &res);
                return res;
            }
            else
            {
                return;
            }
        }

        template<typename ...Args>
        void operator()(Args&& ...args)const { call<void>(std::forward<Args>(args)...); }
    private:

        PyObject* m_object;
    };


    struct py_exception : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
        virtual ~py_exception(){};
    };

}

#endif //PYTHON_UTILS_HPP
