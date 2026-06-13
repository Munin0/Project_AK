// | -------------------------------
#pragma once
// | -------------------------------
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <source_location>
// | -------------------------------

namespace ENG
{
  namespace AnsiColor
  {
    constexpr const char* RESET   = "\033[0m";
    constexpr const char* WHITE   = "\033[37m";
    constexpr const char* CYAN    = "\033[36m";
    constexpr const char* YELLOW  = "\033[33m";
    constexpr const char* RED     = "\033[31m";
    constexpr const char* RED_BG  = "\033[41;97;1m";  // Fondo rojo, texto blanco bold
    constexpr const char* BOLD    = "\033[1m";
    constexpr const char* DIM     = "\033[2m";
  } 

  enum class LogLevel
  {
    INFO,
    DEBUG,
    ERROR,
    FATAL
  };

  class Logger
  {
    public:
      static Logger& Get()
      {
        static Logger instance;
        return instance;
      }

      Logger(const Logger&)            = delete;
      Logger& operator=(const Logger&) = delete;

      void Log(LogLevel level, const std::string& message, const std::source_location& loc = std::source_location::current())
      {
        const std::string timestamp = GetTimestamp();
        const std::string file      = ShortPath(loc.file_name());
        const int         line      = static_cast<int>(loc.line());
        const std::string func      = loc.function_name();

        switch (level)
        {
          case LogLevel::INFO:
            PrintInfo(timestamp, message);
            break;
          case LogLevel::DEBUG:
            PrintDebug(timestamp, message, file, line);
            break;
          case LogLevel::ERROR:
            PrintError(timestamp, message, file, line);
            break;
          case LogLevel::FATAL:
            PrintFatal(timestamp, message, file, line, func);
            std::abort();
            break;
        }
      }
    private:
      Logger() = default;
      static std::string GetTimestamp()
      {
        std::time_t now = std::time(nullptr);
        char buf[10];
        std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&now));
        return buf;
      }

      static std::string ShortPath(const char* fullPath)
      {
        std::string path(fullPath);
        const auto pos = path.find_last_of("/\\");
        return (pos != std::string::npos) ? path.substr(pos + 1) : path;
      }

      void PrintInfo(const std::string& ts, const std::string& msg) const
      {
        std::cout << AnsiColor::DIM    << "[" << ts << "] "
                  << AnsiColor::RESET
                  << AnsiColor::CYAN   << "[INFO]  "
                  << AnsiColor::RESET
                  << AnsiColor::WHITE  << msg
                  << AnsiColor::RESET  << "\n";
      }

      void PrintDebug(const std::string& ts, const std::string& msg, const std::string& file, int line) const 
      {
        std::cout << AnsiColor::DIM    << "[" << ts << "] "
                  << AnsiColor::RESET
                  << AnsiColor::YELLOW << "[DEBUG] "
                  << AnsiColor::RESET
                  << AnsiColor::WHITE  << msg
                  << AnsiColor::DIM    << "  (" << file << ":" << line << ")"
                  << AnsiColor::RESET  << "\n";
      }

      void PrintError(const std::string& ts, const std::string& msg, const std::string& file, int line) const 
      {
        std::cerr << AnsiColor::DIM    << "[" << ts << "] "
                  << AnsiColor::RESET
                  << AnsiColor::RED    << AnsiColor::BOLD << "[ERROR] "
                  << AnsiColor::RESET
                  << AnsiColor::RED    << msg
                  << AnsiColor::DIM    << "  (" << file << ":" << line << ")"
                  << AnsiColor::RESET  << "\n";
      }

      void PrintFatal(const std::string& ts, const std::string& msg, const std::string& file, int line, const std::string& func) const 
      {
        std::cerr << "\n"
                  << AnsiColor::RED_BG
                  << "══════════════════════════════════════════════════"
                  << AnsiColor::RESET  << "\n"
                  << AnsiColor::RED_BG << AnsiColor::BOLD
                  << "  [FATAL]  ERROR ENGINE STOPPING  "
                  << AnsiColor::RESET  << "\n"
                  << AnsiColor::RED_BG
                  << "══════════════════════════════════════════════════"
                  << AnsiColor::RESET  << "\n"
                  << AnsiColor::RED    << AnsiColor::BOLD
                  << "  Mensaje  : " << AnsiColor::RESET << AnsiColor::RED << msg  << "\n"
                  << AnsiColor::RED    << AnsiColor::BOLD
                  << "  Archivo  : " << AnsiColor::RESET << file << ":" << line    << "\n"
                  << AnsiColor::RED    << AnsiColor::BOLD
                  << "  Función  : " << AnsiColor::RESET << func                   << "\n"
                  << AnsiColor::RED    << AnsiColor::BOLD
                  << "  Timestamp: " << AnsiColor::RESET << ts                     << "\n"
                  << AnsiColor::RED_BG
                  << "══════════════════════════════════════════════════"
                  << AnsiColor::RESET  << "\n\n";
      }
  };
#define LOG_INFO(msg)  ENG::Logger::Get().Log(ENG::LogLevel::INFO,  msg, std::source_location::current())
#define LOG_DEBUG(msg) ENG::Logger::Get().Log(ENG::LogLevel::DEBUG, msg, std::source_location::current())
#define LOG_ERROR(msg) ENG::Logger::Get().Log(ENG::LogLevel::ERROR, msg, std::source_location::current())
#define LOG_FATAL(msg) ENG::Logger::Get().Log(ENG::LogLevel::FATAL, msg, std::source_location::current())
}

