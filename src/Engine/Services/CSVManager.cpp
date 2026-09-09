// | -------------------------------
#include "Engine/Services/CSVManager.hpp"
// | -------------------------------
#include "Engine/Extern/rapidcsv.h"
// | -------------------------------
#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
// | -------------------------------

namespace ENG
{
  void CSVManager::Load(const std::string& path, const std::string& key)
  {
    m_csv[key] = std::move(std::make_unique<rapidcsv::Document>());
  }
}
