#include "python_utils.hpp"
#include <compare>
#include <object.h>
#include <pyerrors.h>
#include <pytypedefs.h>
#include <string_view>
#include <unicodeobject.h>

namespace sfw_test
{
    void py_object::throw_current_python_exception()
    {
        const bool exception_occured = PyErr_Occurred();
        assert(exception_occured);
        const std::string message(py_object(PyErr_GetRaisedException()).to_string());
        throw py_exception(message);
    }

    py_object py_object::get_class_constant(py_constant constant)
    {
        return Py_GetConstant(std::to_underlying(constant));
    }

    py_object py_object::load_module(const std::string& module_name)
    {
        PyObject* py_module = PyImport_ImportModule(module_name.c_str());
        if (!py_module)
        {
            throw_current_python_exception();
        }
        return {py_module};
    }

    py_object::py_object()noexcept
        : m_object(NULL) {}

    py_object::py_object(PyObject* obj)noexcept
        : m_object(obj) {}

    py_object::py_object(const py_object& other)noexcept
        : m_object(other.m_object)
    {
        Py_INCREF(m_object);
    }

    py_object& py_object::operator=(const py_object& other)noexcept
    {
        m_object = other.m_object;
        Py_INCREF(m_object);
        return *this;
    }

    py_object::py_object(py_object&& other)noexcept
        : m_object(other.m_object)
    {
        other.m_object = nullptr;
    }

    py_object& py_object::operator=(py_object&& other)noexcept
    {
        m_object = other.m_object;
        other.m_object = nullptr;
        return *this;
    }

    py_object& py_object::operator=(PyObject* obj) noexcept
    {
        m_object = obj;
        return *this;
    }

    py_object::py_object(std::string_view data)noexcept
        : m_object(PyUnicode_DecodeASCII(data.data(), data.size(), nullptr)) {}

    py_object::~py_object()
    {
        Py_XDECREF(m_object);
    }

    py_object::operator PyObject*()const noexcept { return m_object; }

    void py_object::print(std::filesystem::path file, int flags)
    {
        std::source_location src = std::source_location::current();
        SFW_LOG_ERROR("Python", "{}:{} not implemented", src.file_name(), src.function_name());
    }

    [[nodiscard]]
    bool py_object::has_attribute(const std::string& name) const
    {
        int result = PyObject_HasAttrStringWithError(m_object, name.c_str());
        switch (result)
        {
            case 1: return true;
            case 0: return false;
            case -1: throw_current_python_exception();
            default: assert(false);
        }
    }

    [[nodiscard]]
    bool py_object::is_callable()const noexcept
    {
        return PyCallable_Check(m_object);
    }

    [[nodiscard]]
    std::string_view py_object::to_string()const
    {
        PyObject* str_repr = PyObject_Str(m_object);
        Py_ssize_t str_size = 0;
        const char* str = PyUnicode_AsUTF8AndSize(str_repr, &str_size);

        if (str_size == -1)
        {
            throw_current_python_exception();
        }
        return {str, static_cast<size_t>(str_size)};
    }

    std::optional<py_object> py_object::get_attribute(const std::string& name) const
    {
        PyObject* out_obj = nullptr;
        int result = PyObject_GetOptionalAttrString(m_object, name.c_str(), &out_obj);

        switch (result)
        {
            case 1: return {out_obj};
            case 0: return {};
            case -1: throw_current_python_exception();
            default: assert(false);
        }
    }
}