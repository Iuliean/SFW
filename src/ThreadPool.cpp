#include <cstddef>
#include <exception>
#include <thread>
#include <iostream>

#include "LoggerManager.h"
#include "ThreadPool.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace iu
{
    ThreadPool::ThreadPool(size_t maxThreads, const std::string& name)
        :m_logger(LoggerManager::GetLogger("ThreadPool"))
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
                m_logger.debug("Starting Task");
                (*fn)();            
                m_logger.debug("Task Stopped");
            }
            catch(...)
            {
                m_logger.debug("Task Stopped");
                m_logger.error("Exception thrown by task!!!!");
            }
        }
    }
}