#include "log.hpp"
#include <cstdarg>
#include <libgen.h>
#include <spdlog/logger.h>
//#include "spdlog/spdlog.h"
//#include "spdlog/fmt/ostr.h" // support for user defined types

Logger::Logger()
{

}

void Logger::SetLogLevel(int log_level)
{
    switch (log_level)
    {
        case LogLevel::TRACE_LEVEL:
            spdlog::set_level(spdlog::level::trace);
            break;
        
        case LogLevel::DEBUG_LEVEL:
            spdlog::set_level(spdlog::level::debug);
            break;
        
        case LogLevel::INFO_LEVEL:
            spdlog::set_level(spdlog::level::info);
            break;
        
        case LogLevel::WARN_LEVEL:
            spdlog::set_level(spdlog::level::warn);
            break;
        
        case LogLevel::ERROR_LEVEL:
            spdlog::set_level(spdlog::level::err);
            break;
        
        case LogLevel::CRITICAL_LEVEL:
            spdlog::set_level(spdlog::level::critical); //
            break;

        default :
            spdlog::set_level(spdlog::level::off);
            break;
    }
}

void Logger::TraceLog(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    SPDLOG_TRACE(format, ap);
    va_end(ap);
}

void Logger::DebugLog(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    SPDLOG_DEBUG(format, ap);
    va_end(ap);
}

void Logger::InfoLog(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    SPDLOG_INFO(format, ap);
    va_end(ap);
}

void Logger::WarningLog(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    SPDLOG_ERROR(format, ap);
    va_end(ap);
}

void Logger::ErrorLog(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    spdlog::error(format, ap);
    va_end(ap);
}

void Logger::CriticalLog(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    spdlog::critical(format, ap);
    va_end(ap);
}

Logger::~Logger()
{

}