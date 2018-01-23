#ifndef LOGGER_H_
#define LOGGER_H_

#include <cstdint>
#include <string>

class Logger
{
 public:
  Logger() = delete;

  enum class Level : std::size_t
  {
    // Programming errors / bugs / "this should never occur"
    ERROR    = 0,

    // Critical problems that usually leads to application termination
    CRITICAL = 1,

    // For normal non-repetitive events. For humans
    INFO     = 2,

    // For any type of events. For developers
    DEBUG    = 3,
  };

  static void log(const char* full_filename, int line, Level level, ...);
};

#define LOG_ERROR(...)    Logger::log(__FILE__, __LINE__, Logger::Level::ERROR, __VA_ARGS__)
#define LOG_CRITICAL(...) Logger::log(__FILE__, __LINE__, Logger::Level::CRITICAL, __VA_ARGS__)
#define LOG_INFO(...)     Logger::log(__FILE__, __LINE__, Logger::Level::INFO,  __VA_ARGS__)

#ifndef NDEBUG
#define LOG_DEBUG(...)    Logger::log(__FILE__, __LINE__, Logger::Level::DEBUG, __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#endif  // LOGGER_H_
