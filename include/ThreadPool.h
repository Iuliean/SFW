#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <atomic>
#include <memory>
#include <queue>
#include <vector>
#include <thread>
#include <type_traits>
#include <mutex>
#include <condition_variable>


#include "Callback.h"
#include "Task.h"
#include "Runnable.h"

namespace iu
{
    class ThreadPool
    {
    public:
        ThreadPool(size_t maxThreads);
        //stops all threads before destruction
        ~ThreadPool();
        template<typename T, typename ...Args>
        void AddTask(Callback<T> callback, Args&& ...args)
        {
            std::lock_guard l(m_taskQueueMutex);
            std::unique_ptr<Runnable> p = std::make_unique<Task<T, Args...>>(callback, std::move(args)...);
            m_taskQueue.push(std::move(p));
            m_cv.notify_one();
        }
    private:
        void threadEntry();    

    private:
    std::atomic_bool m_stop;
    std::vector<std::thread> m_threads;
    std::mutex m_taskQueueMutex;
    std::condition_variable m_cv;
    std::queue<std::unique_ptr<Runnable>> m_taskQueue;
    };
}

#endif //THREAD_POOL_H