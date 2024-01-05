#ifndef CALLBACK_H
#define CALLBACK_H
#include <concepts>
#include <type_traits>

#include "Connection.h"
namespace iu
{
    template<typename Callable>   
    class Callback
    {
    public:

        Callback(Callable f)
            :m_func(f)
        {}
        ~Callback() = default;
        
        void operator()(Connection&& conn)const
        {
            m_func(std::move(conn));
        }
    private:
        Callable m_func;
    };

    template<typename T>
    class Callback<void (T::*)(Connection&&)> 
    {
    public:
        typedef void(T::*MethodPtr)(Connection&&);

        Callback(MethodPtr m, T* instance)
            : m_func(m), m_instance(instance)
        {}
        ~Callback() = default;

        void operator()(Connection&& conn)const
        {
            (*m_instance.*m_func)(std::move(conn));
        }
    private:
        MethodPtr m_func;
        T* m_instance;
    };

    template<typename T>
    Callback(void (T::*)(Connection&&), T*) -> Callback<void (T::*)(Connection&&)>;
}

#endif //CALLBACK_H