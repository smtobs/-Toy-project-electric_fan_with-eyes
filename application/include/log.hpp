#ifndef LOG_H
#define LOG_H

enum class LogLevel : int 
{ 
    OFF_LEVEL = 0,
    CRITICAL_LEVEL,
    ERROR_LEVEL,
    WARN_LEVEL,
    INFO_LEVEL,
    DEBUG_LEVEL,
    TRACE_LEVEL
};

class Logger
{
public:
    Logger();
    void SetLogLevel(int log_level);
    void TraceLog(const char* format, ...);
    void DebugLog(const char* format, ...);
    void InfoLog(const char* format, ...);
    void WarningLog(const char* format, ...);
    void ErrorLog(const char* format, ...);
    void CriticalLog(const char* format, ...);
    ~Logger();
private:

};
#endif