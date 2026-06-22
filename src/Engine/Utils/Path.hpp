// | -------------------------------
#pragma once
// | -------------------------------
#include <filesystem>
#include <string>
// | -------------------------------

namespace ENG
{
  class Path
  {
    public:
      static Path& Get();
      static void Destroy();

      std::string ReadFile(const std::filesystem::path& path) const;

      const std::filesystem::path& ExecutableDir() const { return _executableDir; }

      std::filesystem::path AssetsPath;
      std::filesystem::path ShadersPath;
      std::filesystem::path ConfigPath;

    private:
      Path();
      ~Path() = default;
      Path(const Path&) = delete;
      Path& operator=(const Path&) = delete;

      std::filesystem::path GetExecutableDir() const;

      std::filesystem::path _executableDir;
      static inline Path* _instance = nullptr;
  };
}
