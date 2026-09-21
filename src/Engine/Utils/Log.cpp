// | -------------------------------
#include "Log.hpp"
// | -------------------------------
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <source_location>
#include <string>
// | -------------------------------

namespace ENG
{
  Logger& Logger::Get()
  {
    if(!m_instance)
    {
      m_instance = new Logger();
    }
    return *m_instance;
  }

  void Logger::Log(LogLevel level, const std::string& message, const std::source_location& loc)
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

  std::string Logger::GetTimestamp()
  {
    std::time_t now = std::time(nullptr);
    char buf[10];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&now));
    return buf;
  }

  std::string Logger::ShortPath(const char* fullPath)
  {
    std::string path(fullPath);
    const auto pos = path.find_last_of("/\\");
    return (pos != std::string::npos) ? path.substr(pos + 1) : path;
  }

  void Logger::PrintInfo(const std::string& ts, const std::string& msg) const
  {
    std::cout << DIM << "[" << ts << "] " << RESET << CYAN   << "[INFO]  " << RESET << WHITE  << msg << RESET  << "\n";
  }

  void Logger::PrintDebug(const std::string& ts, const std::string& msg, const std::string& file, int line) const 
  {
    std::cout << DIM << "[" << ts << "] " << RESET << YELLOW << "[DEBUG] " << RESET << WHITE  << msg << DIM    << "  (" << file << ":" << line << ")" << RESET  << "\n";
  }

  void Logger::PrintError(const std::string& ts, const std::string& msg, const std::string& file, int line) const
  {
    std::cerr << DIM << "[" << ts << "] " << RESET << RED << BOLD << "[ERROR] " << RESET << RED << msg << DIM << "  (" << file << ":" << line << ")" << RESET  << "\n";
  }

  void Logger::PrintFatal(const std::string& ts, const std::string& msg, const std::string& file, int line, const std::string& func) const
  {
    std::cerr << "\n"
      << RED_BG
      << "══════════════════════════════════════════════════"
      << RESET  << "\n"
      << RED_BG << BOLD
      << "  [FATAL]  ERROR ENGINE STOPPING  "
      << RESET  << "\n"
      << RED_BG
      << "══════════════════════════════════════════════════"
      << RESET  << "\n"
      << RED    << BOLD
      << "  Mensaje  : " << RESET << RED << msg  << "\n"
      << RED    << BOLD
      << "  Archivo  : " << RESET << file << ":" << line    << "\n"
      << RED    << BOLD
      << "  Función  : " << RESET << func                   << "\n"
      << RED    << BOLD
      << "  Timestamp: " << RESET << ts                     << "\n"
      << RED_BG
      << "══════════════════════════════════════════════════"
      << RESET  << "\n\n";
  }

}
