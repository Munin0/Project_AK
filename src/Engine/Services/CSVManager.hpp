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
      void Load(const std::string& path, const std::string& key, int colID = 0, int rowID = 0);
      std::string GetText(const std::string& language, const std::string& dialog, const std::string& key);
      std::vector<std::string> GetRangeText(const std::string& language, const std::string& from, const std::string& where, const std::string& key);
      void Clear();
    private:
      rapidcsv::Document* GetDoc(const std::string& key);

      std::map<std::string, std::unique_ptr<rapidcsv::Document>> m_csv;
  };
}
