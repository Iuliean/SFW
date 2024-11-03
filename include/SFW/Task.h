 #ifndef TASK_H
#define TASK_H
#include "Callback.h"
#include "Runnable.h"

namespace iu
{
    template<typename T, typename ...Args>
    class Task : public Runnable
    {
    public:
        Task(Callback<T> callback, Args&& ...args)
            : m_callback(callback), m_args(args...)
        {

        }
        ~Task() override = default;

        void operator()()override
        {
            m_callback(std::forward<Args>(std::get<Args>(m_args))...);
        }

    private:
        Callback<T> m_callback;
        std::tuple<Args...> m_args;
    };
}

#endif //TASK_H