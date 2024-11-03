#ifndef LOGGER_H
#define LOGGER_H
#include "spdlog/common.h"
#include "spdlog/logger.h"
#include <map>
#include <memory>
#include <spdlog/sinks/sink.h>


namespace iu
{

    enum class LogLevel
    {
        DEBUG   = spdlog::level::debug,
        INFO    = spdlog::level::info,
        WARNING = spdlog::level::warn,
        ERROR   = spdlog::level::err
    };

    class Logger
    {
    public:
        template<typename... Args>
        using format_string_t = spdlog::format_string_t<Args...>;

        Logger(std::shared_ptr<spdlog::logger> logger);
        ~Logger();
        
        template<typename... Args>
        inline void debug(format_string_t<Args...> fmt, Args &&... args)const
        {
            m_logger->debug(fmt, args...);
        }

        template<typename... Args>
        inline void info(format_string_t<Args...> fmt, Args &&... args)const
        {
            m_logger->info(fmt, args...);
        }
        
        template<typename... Args>
        inline void warn(format_string_t<Args...> fmt, Args &&... args)const
        {
            m_logger->warn(fmt, args...);
        }

        template<typename... Args>
        inline void error(format_string_t<Args...> fmt, Args &&... args)const
        {
            m_logger->error(fmt, args...);
        }

    private:
        std::shared_ptr<spdlog::logger> m_logger;
    };

}

#endif //LOGGER_H