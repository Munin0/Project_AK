// | -------------------------------
#include "IOPrimitives.hpp"
// | -------------------------------
#include <cstdint>
#include <fstream>
#include <string>
// | -------------------------------

namespace ENG
{
  void WriteString(std::ofstream& f, const std::string& s)
  {
    uint32_t len = static_cast<uint32_t>(s.size());
    WritePOD(f, len);
    f.write(s.data(), len);
  }

  std::string ReadString(std::ifstream& f)
  {
    uint32_t len;
    ReadPOD(f, len);
    std::string s(len, '\0');
    f.read(s.data(), len);
    return s;
  }
}
