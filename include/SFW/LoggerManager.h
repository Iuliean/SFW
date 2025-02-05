#ifndef LOGGER_MANAGER_H
#define LOGGER_MANAGER_H
#include <memory>
#include <string>
#include <unordered_map>
#include "spdlog/common.h"
#include "spdlog/logger.h"

#include "utils.h"
namespace iu
{
    class SFW_API LoggerManager
    {
    public:
        using Logger = spdlog::logger;
        using LogLevel = spdlog::level::level_enum;

        static void LogToConsole();
        static void SetLevel(LogLevel level);
        static void LogFile(const std::string& fileName);
        static void RotatingFile(const std::string& fileName, size_t maxSize, size_t maxFileCount);

        static std::shared_ptr<Logger> GetLogger(const std::string& name);
    private:
        static std::vector<spdlog::sink_ptr> s_sinks;
        inline static LogLevel level = LogLevel::info;
    };
}

#define SFW_LOGGER(DOM) iu::LoggerManager::GetLogger(DOM)

#define SFW_LOG_DEBUG(DOM, FMT,...) {SFW_LOGGER(DOM)->debug(FMT, ##__VA_ARGS__);}
#define SFW_LOG_INFO(DOM, FMT,...) {SFW_LOGGER(DOM)->info(FMT, ##__VA_ARGS__);}
#define SFW_LOG_WARN(DOM, FMT,...) {SFW_LOGGER(DOM)->warn(FMT, ##__VA_ARGS__);}
#define SFW_LOG_ERROR(DOM, FMT,...) {SFW_LOGGER(DOM)->error(FMT, ##__VA_ARGS__);}

#endif //LOGGER_MANAGER_H