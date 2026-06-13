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
      void DestroyPath(void) const;
      std::string ReadFile(const std::string& path);

      std::filesystem::path AssetsPath;
      std::filesystem::path ShadersPath;
      std::filesystem::path ConfigPath;
    private:
      explicit Path() = default;
      static inline Path* _Path = nullptr;
  };
}
