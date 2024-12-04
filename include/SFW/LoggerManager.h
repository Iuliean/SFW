#ifndef LOGGER_MANAGER_H
#define LOGGER_MANAGER_H
#include <memory>
#include "spdlog/common.h"

#include "Logger.h"

namespace iu
{
    class SFW_API LoggerManager
    {
    public:
        static void Init();
        static void SetLevel(LogLevel level);
        static void LogFile(const std::string& fileName);
        static void RotatingFile(const std::string& fileName, size_t maxSize, size_t maxFileCount);

        static Logger GetLogger(const std::string& name);
        static Logger& GlobalLogger();
    private:
        static std::vector<spdlog::sink_ptr> s_sinks;
        static std::unique_ptr<Logger> s_globalScopeLogger;
    };
}

#endif //LOGGER_MANAGER_H