// | -------------------------------
#include "Path.hpp"
// | -------------------------------
#include <filesystem>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstddef>
// | -------------------------------
#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <linux/limits.h>
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#endif
// | -------------------------------

namespace ENG
{
  std::filesystem::path Path::GetExecutableDir() const
  {
#if defined(_WIN32)
    char result[MAX_PATH];
    DWORD count = GetModuleFileNameA(nullptr, result, MAX_PATH);
    if(count == 0)
    {
      throw std::runtime_error("Path: failed to resolve executable path (Win32)");
    }
    return std::filesystem::path(std::string(result, count)).parent_path();

#elif defined(__linux__)
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    if(count == -1)
    {
      throw std::runtime_error("Path: failed to resolve executable path (/proc/self/exe)");
    }
    return std::filesystem::path(std::string(result, static_cast<size_t>(count))).parent_path();

#else
  #error "Path::GetExecutableDir: platform not supported"
#endif
  }

  Path::Path()
  {
    _executableDir = GetExecutableDir();

    // Ajusta estas rutas a la estructura real de Project-AK.
    AssetsPath  = _executableDir / "assets/";
    ShadersPath = _executableDir / "shaders/";
    ConfigPath  = _executableDir / "config/";
  }

  Path& Path::Get()
  {
    if(!_instance)
    {
      _instance = new Path();
    }
    return *_instance;
  }

  void Path::Destroy()
  {
    delete _instance;
    _instance = nullptr;
  }

  std::string Path::ReadFile(const std::filesystem::path& path) const
  {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if(!file.is_open())
    {
      throw std::runtime_error("Path::ReadFile: could not open file: " + path.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }
}
