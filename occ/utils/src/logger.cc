#include "logger.h"

#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <cstring>

namespace
{
  const char* level_to_string(Logger::Level level)
  {
    static const char* error    = "ERROR";
    static const char* critical = "CRITICAL";
    static const char* info     = "INFO";
    static const char* debug    = "DEBUG";
    static const char* invalid  = "INVALID_LOG_LEVEL";

    switch (level)
    {
      case Logger::Level::ERROR:    return error;
      case Logger::Level::CRITICAL: return critical;
      case Logger::Level::INFO:     return info;
      case Logger::Level::DEBUG:    return debug;
      default:                      return invalid;
    }
  }
}

void Logger::log(const char* full_filename, int line, Level level, ...)
{
  // Remove directories in filename
  const char* filename;
  if (strrchr(full_filename, '/'))
  {
    filename = strrchr(full_filename, '/') + 1;
  }
  else
  {
    filename = full_filename;
  }

  // Get current date and time
  auto now = time(0);
  tm time_struct{};
  char time_str[32];
  localtime_r(&now, &time_struct);
  strftime(time_str, sizeof(time_str), "%Y-%m-%d %X", &time_struct);

  // Extract variadic function arguments
  va_list args;
  va_start(args, level);
  auto* format = va_arg(args, const char*);
  char message[256];
  vsnprintf(message, sizeof(message), format, args);
  va_end(args);

  printf("[%s][%s:%d] %s: %s\n", time_str, filename, line, level_to_string(level), message);
}
