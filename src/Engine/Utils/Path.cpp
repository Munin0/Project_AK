/// | ------------------------------------ |
#include "Path.hpp"
/// | ------------------------------------ |
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <filesystem>
#include <sys/types.h>
/// | ------------------------------------ |

namespace ENG
{
  static std::filesystem::path GetExecutableDir()
  {
    char result[256];
    ssize_t count = readlink("/proc/self/exe", result, 256);
    return std::filesystem::path(std::string(result, count)).parent_path();
  }

  std::string Path::ReadFile(const std::string& _)
  {
    std::ifstream file(_);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }
  
  Path& Path::Get()
  {
    if(!_Path)
    {
      _Path = new Path();
    }
    return *_Path;
  }
}
