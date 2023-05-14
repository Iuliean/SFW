#include <cstddef>
#include <exception>
#include <thread>
#include <iostream>

#include "ThreadPool.h"

namespace iu
{
    ThreadPool::ThreadPool(size_t maxThreads)
    {
        m_threads.reserve(maxThreads);
        m_stop = false;
        for(size_t i = 0; i < maxThreads; i++)
            m_threads.emplace_back(&ThreadPool::threadEntry, this);
    
    }

    ThreadPool::~ThreadPool()
    {
        m_stop= true;
        m_cv.notify_all();
        for(auto& thread : m_threads)
            thread.join();
    }

    void ThreadPool::threadEntry()
    {
        while(!m_stop.load())
        {
            std::unique_lock ul(m_taskQueueMutex);
            while(m_taskQueue.empty() && !m_stop.load())
                m_cv.wait(ul,[this](){ return !m_taskQueue.empty() || m_stop.load();});

            if(m_stop.load())
                return;

            auto fn = std::move(m_taskQueue.front());
            m_taskQueue.pop();

            ul.unlock();
            try{
                (*fn)();            
            }
            catch(...)
            {
                std::cerr << "Exception\n";
            }
        }
    }
}