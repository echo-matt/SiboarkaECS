#pragma once
#include <iostream>
#include <string>
#include <fstream>

class Logger
{
public:
    enum class Level {Trace, Debug, Info, Warn, Error, Fatal};
    
    static Logger& get()
    {
        static Logger instance;
        return instance;
    }
    
    void setLevel(Level minLevel) {m_minLevel = minLevel;}
    
    void log(Level level, const std::string& message)
    {
        if (level < m_minLevel) return;
        
        const char* prefix = levelToString(level);
        printf("[%s] %s\n", prefix, message.c_str());
    }
    
    void trace(const std::string& msg) { log(Level::Trace, msg); }
    void debug(const std::string& msg) { log(Level::Debug, msg); }
    void info (const std::string& msg) { log(Level::Info,  msg); }
    void warn (const std::string& msg) { log(Level::Warn,  msg); }
    void error(const std::string& msg) { log(Level::Error, msg); }
    void fatal(const std::string& msg) { log(Level::Fatal, msg); }
    
    
private:
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    Level m_minLevel = Level::Debug;
    
    const char* levelToString(Level l)
    {
        switch (l)
        {
        case Level::Trace: return "TRACE";
        case Level::Debug: return "DEBUG";
        case Level::Info:  return "INFO ";
        case Level::Warn:  return "WARN ";
        case Level::Error: return "ERROR";
        case Level::Fatal: return "FATAL";
        default:           return "?????";
        }
    }
};

#define SIBOLOG_TRACE(msg) Logger::get().trace(msg)
#define SIBOLOG_DEBUG(msg) Logger::get().debug(msg)
#define SIBOLOG_INFO(msg)  Logger::get().info(msg)
#define SIBOLOG_WARN(msg)  Logger::get().warn(msg)
#define SIBOLOG_ERROR(msg) Logger::get().error(msg)
#define SIBOLOG_FATAL(msg) Logger::get().fatal(msg)