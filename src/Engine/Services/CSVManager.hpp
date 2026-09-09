// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Extern/rapidcsv.h"
// | -------------------------------
#include <map>
#include <memory>
#include <string>
#include <vector>
// | -------------------------------

namespace ENG
{
  class CSVManager
  {
    public:
      void Load(const std::string& path, const std::string& key);
      std::string GetText(int lenguage, int dialog, const std::string& key);
      std::vector<std::string> GetRangeText(int language, int rangeA, int rangeB, const std::string& key);
      void Clear();
    private:
      std::map<std::string, std::unique_ptr<rapidcsv::Document>> m_csv;
  };
}
