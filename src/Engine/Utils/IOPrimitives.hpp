// | -------------------------------
#pragma once
// | -------------------------------
#include <cstdint>
#include <fstream>
#include <optional>
#include <vector>
// | -------------------------------

namespace ENG
{
  template<typename T>
    void WritePOD(std::ofstream& f, const T& val)
    {
      static_assert(std::is_trivially_copyable_v<T>);
      f.write(reinterpret_cast<const char*>(&val), sizeof(T));
    }

  template<typename T>
    void ReadPOD(std::ifstream& f, T& val)
    {
      static_assert(std::is_trivially_copyable_v<T>);
      f.read(reinterpret_cast<char*>(&val), sizeof(T));
    }

        
  template<typename T>
    void WriteOptional(std::ofstream& f, const std::optional<T>& opt, void(*writeFn)(std::ofstream&, const T&))
    {
      bool has = opt.has_value();
      WritePOD(f, has);
      if (has)
        writeFn(f, *opt);
    }

  template<typename T>
    void ReadOptional(std::ifstream& f, std::optional<T>& opt, T(*readFn)(std::ifstream&))
    {
      bool has;
      ReadPOD(f, has);
      if (has)
        opt = readFn(f);
      else
        opt.reset();
    }

  template<typename T>
    void WriteVector(std::ofstream& f, const std::vector<T>& vec, void(*writeFn)(std::ofstream&, const T&))
    {
      uint32_t count = static_cast<uint32_t>(vec.size());
      WritePOD(f, count);
      for (auto& item : vec) 
        writeFn(f, item);
    }

  template<typename T>
    std::vector<T> ReadVector(std::ifstream& f, T(*readFn)(std::ifstream&))
    {
      uint32_t count;
      ReadPOD(f, count);
      std::vector<T> vec;
      vec.reserve(count);
      for (uint32_t i = 0; i < count; ++i)
        vec.push_back(readFn(f));
      return vec;
    }
}
