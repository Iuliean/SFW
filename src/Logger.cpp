#include "Logger.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <mutex>

namespace iu
{
    Logger::Logger(std::shared_ptr<spdlog::logger> logger)
        :m_logger(std::move(logger))
    {
    }

    Logger::~Logger()
    {
        if(m_logger.use_count() == 2)
        {
            spdlog::drop(m_logger->name());
        }
    }
}