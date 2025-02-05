#include "LoggerManager.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "utils.h"
#include <memory>
#include <mutex>
#include <string_view>

static constexpr auto LOG_FORMAT = "[%d/%m/%Y %T.%e][%t][%n][%^%l%$]: %v";

namespace iu
{
    std::vector<spdlog::sink_ptr> LoggerManager::s_sinks;

    void LoggerManager::LogToConsole()
    {
        s_sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    }

    void LoggerManager::SetLevel(LogLevel level)
    {
        for(auto& sink : s_sinks)
        {
            sink->set_level(static_cast<spdlog::level::level_enum>(level));
        }
        spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
    }

    void LoggerManager::LogFile(const std::string &fileName)
    {
        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fileName);
        s_sinks.push_back(std::move(sink));
    }

    void LoggerManager::RotatingFile(const std::string &fileName, size_t maxSize, size_t maxFileCount)
    {
        auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(fileName, maxSize, maxFileCount);
        s_sinks.push_back(std::move(sink));
    }

    std::shared_ptr<LoggerManager::Logger> LoggerManager::GetLogger(const std::string& name)
    {
        if(spdlog::get(name) == nullptr)
        {
            auto logger = std::make_shared<spdlog::logger>(name, s_sinks.begin(), s_sinks.end());
            logger->set_pattern(LOG_FORMAT);
            logger->set_level(s_sinks[0]->level());
            spdlog::register_logger(logger);
            return logger;
        }
        else
        {
            return spdlog::get(name);
        }
    }
}