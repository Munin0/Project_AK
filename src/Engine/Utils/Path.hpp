// | -------------------------------
#pragma once
// | -------------------------------
#include <filesystem>
#include <sstream>
#include <string>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Central access point for the engine's directories and file reading.
   *
   * Path is a singleton that resolves the location of the engine's resource
   * directories (assets, shaders, config and data) starting from the
   * directory of the executable, so resources can be found regardless of
   * the current working directory. It also provides helpers to read whole
   * files into memory.
   *
   * Usage:
   * @code
   * std::string src = ENG::Path::Get().ReadFileString(ENG::Path::Get().ShadersPath / "basic.vert");
   * @endcode
   *
   * @note The class is not copyable or assignable.
   */
  class Path
  {
    public:
      /**
       * @brief Returns the global Path instance.
       * @return Reference to the singleton instance.
       */
      static Path& Get();
      /**
       * @brief Destroys the global Path instance.
       *
       * Must be called once, at engine shutdown. Any reference previously
       * obtained through Get() becomes invalid after this call.
       */
      static void Destroy();
      /**
       * @brief Reads a whole file and returns its content as a string.
       *
       * @param[in] path Path of the file to read.
       * @return The content of the file as a std::string.
       *
       * @see ReadFile() to get the content as a std::stringstream.
       */
      std::string ReadFileString(const std::filesystem::path& path) const;
      /**
       * @brief Reads a whole file and returns its content as a stream.
       *
       * @param[in] path Path of the file to read.
       * @return The content of the file inside a std::stringstream.
       *
       * @see ReadFileString() to get the content as a std::string.
       */
      std::stringstream ReadFile(const std::filesystem::path& path) const;
      /**
       * @brief Returns the base directory used to resolve the engine's paths.
       *
       * This directory is derived from the location of the executable.
       *
       * @return Reference to the executable's base directory.
       */
      const std::filesystem::path& ExecutableDir() const { return m_executableDir; }
      /// @brief Assets directory ("Assets/"), resolved from ExecutableDir().
      std::filesystem::path AssetsPath;
      /// @brief Shaders directory ("Shaders/"), resolved from ExecutableDir().
      std::filesystem::path ShadersPath;
      /// @brief Config directory ("Config/"), resolved from ExecutableDir().
      std::filesystem::path ConfigPath;
      /// @brief Data directory ("Data/"), resolved from ExecutableDir().
      std::filesystem::path DataPath;
    private:
      /**
       * @brief Default constructor. Private: the instance is managed through Get().
       *
       * Locates the executable's directory and builds the resource paths
       * (AssetsPath, ShadersPath, ConfigPath and DataPath) from it.
       */
      Path();
      /// @brief Default destructor. Private: the instance is released through Destroy().
      ~Path() = default;
      /// @brief Copying is not allowed: there is a single Path instance.
      Path(const Path&) = delete;
      /// @brief Assignment is not allowed: there is a single Path instance.
      Path& operator=(const Path&) = delete;
      /**
       * @brief Finds the base directory of the running executable.
       *
       * Starts from the location of the executable and moves up to the
       * directory used as the root of the resource folders.
       *
       * @return The base directory.
       */
      std::filesystem::path GetExecutableDir() const;
    private:
      /// @brief Base directory derived from the executable's location.
      std::filesystem::path m_executableDir;
      /// @brief Pointer to the global Path instance. nullptr while no instance exists.
      static inline Path* m_instance = nullptr;
  };
}
