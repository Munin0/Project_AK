// | -------------------------------
#pragma once
// | -------------------------------
#include <string>
#include <source_location>
// | -------------------------------
#define LOG_INFO(msg)  ENG::Logger::Get().Log(ENG::LogLevel::INFO,  msg, std::source_location::current())
#define LOG_DEBUG(msg) ENG::Logger::Get().Log(ENG::LogLevel::DEBUG, msg, std::source_location::current())
#define LOG_ERROR(msg) ENG::Logger::Get().Log(ENG::LogLevel::ERROR, msg, std::source_location::current())
#define LOG_FATAL(msg) ENG::Logger::Get().Log(ENG::LogLevel::FATAL, msg, std::source_location::current())
// | -------------------------------

namespace ENG
{
  /**
   * @brief Severity levels supported by the Logger.
   */
  enum class LogLevel
  {
    INFO,   ///< General information about the normal flow of the engine.
    DEBUG,  ///< Diagnostic messages useful during development.
    ERROR,  ///< An error occurred, but the engine keeps running.
    FATAL   ///< Unrecoverable error. The engine is closed after printing it.
  };
  /**
   * @brief Console logger of the engine.
   *
   * Logger is a singleton that prints colored messages to the console. Each
   * message is printed with a timestamp and a severity level. DEBUG, ERROR
   * and FATAL messages also include the source file and line of the call
   * site, and FATAL messages include the calling function name as well.
   *
   * The call site information is captured automatically through
   * std::source_location, so callers only need to pass the level and the
   * message.
   *
   * Usage:
   * @code
   * ENG::Logger::Get().Log(ENG::LogLevel::INFO, "Engine started");
   * @endcode
   *
   * @note The class is not copyable or assignable.
   */
  class Logger
  {
    public:
      /**
       * @brief Returns the global Logger instance.
       * @return Reference to the singleton instance.
       */
      static Logger& Get();
      /// @brief Copying is not allowed: there is a single Logger instance.
      Logger(const Logger&)            = delete;
      /// @brief Assignment is not allowed: there is a single Logger instance.
      Logger& operator=(const Logger&) = delete;
      /**
       * @brief Prints a message with the given severity level.
       *
       * Main entry point of the Logger. It builds the timestamp and the
       * short file name, then redirects to the print function that matches
       * the level.
       *
       * @param[in] level   Severity of the message.
       * @param[in] message Text to print.
       * @param[in] loc     Source location of the call. Filled in
       *                    automatically with the call site; do not pass it
       *                    manually.
       *
       * @warning A message with LogLevel::FATAL closes the engine.
       */
      void Log(LogLevel level, const std::string& message, const std::source_location& loc = std::source_location::current());
    private:
      /// @brief Default constructor. Private: the instance is managed through Get().
      Logger() = default;
      /**
       * @brief Returns the current local time of the machine as text.
       * @return The timestamp, formatted as a string.
       */
      std::string GetTimestamp();
      /**
       * @brief Shortens a full file path to its last section.
       *
       * Removes the directories from the path and keeps only the file name.
       *
       * @param[in] fullPath Full path, as given by std::source_location::file_name().
       * @return The file name without its directories.
       */
      std::string ShortPath(const char* fullPath);
      /**
       * @brief Prints an INFO message.
       *
       * @param[in] ts  Timestamp of the message.
       * @param[in] msg Text to print.
       */
      void PrintInfo(const std::string& ts, const std::string& msg) const;
      /**
       * @brief Prints a DEBUG message.
       *
       * @param[in] ts   Timestamp of the message.
       * @param[in] msg  Text to print.
       * @param[in] file Short name of the source file that emitted the message.
       * @param[in] line Line of the source file that emitted the message.
       */
      void PrintDebug(const std::string& ts, const std::string& msg, const std::string& file, int line) const;
      /**
       * @brief Prints an ERROR message.
       *
       * @param[in] ts   Timestamp of the message.
       * @param[in] msg  Text to print.
       * @param[in] file Short name of the source file that emitted the message.
       * @param[in] line Line of the source file that emitted the message.
       */
      void PrintError(const std::string& ts, const std::string& msg, const std::string& file, int line) const;
      /**
       * @brief Prints a FATAL message and closes the engine.
       *
       * @param[in] ts   Timestamp of the message.
       * @param[in] msg  Text to print.
       * @param[in] file Short name of the source file that emitted the message.
       * @param[in] line Line of the source file that emitted the message.
       * @param[in] func Name of the function that emitted the message.
       */
      void PrintFatal(const std::string& ts, const std::string& msg, const std::string& file, int line, const std::string& func) const;
    private:
      /// @brief Pointer to the global Logger instance. nullptr while no instance exists.
      static inline Logger* m_instance = nullptr;
    public:
      /// @name Text colors (ANSI escape codes)
      /// @{
      const char* RESET   = "\033[0m";   ///< Resets all colors and styles.
      const char* WHITE   = "\033[37m";  ///< White text.
      const char* CYAN    = "\033[36m";  ///< Cyan text.
      const char* YELLOW  = "\033[33m";  ///< Yellow text.
      const char* RED     = "\033[31m";  ///< Red text.
      const char* BOLD    = "\033[1m";   ///< Bold style.
      const char* DIM     = "\033[2m";   ///< Dimmed (faint) style.
      /// @}

      /// @name Background colors (ANSI escape codes)
      /// @{
      const char* RED_BG  = "\033[41;97;1m"; ///< Red background with bold bright white text.
      /// @}
  };
}
