// | -------------------------------
#include "Engine/Services/CSVManager.hpp"
// | -------------------------------
#include "Engine/Extern/rapidcsv.h"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
#include <cstddef>
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
// | -------------------------------

namespace ENG
{
  void CSVManager::Clear()
  {
    m_csv.clear();
  }

  void CSVManager::Load(const std::string& path, const std::string& key, int colID, int rowID)
  {
    auto p = Path::Get().AssetsPath / path;
    m_csv[key] = std::move(std::make_unique<rapidcsv::Document>(p.string(), rapidcsv::LabelParams(colID, rowID)));
  }

  rapidcsv::Document* CSVManager::GetDoc(const std::string& key)
  {
    auto it = m_csv.find(key);
    if (it == m_csv.end())
    {
      LOG_ERROR(" | << Key not found: " + key);
      return nullptr;
    }
    return it->second.get();

  }

  std::string CSVManager::GetText(const std::string& language, const std::string& dialog, const std::string& key)
  {
    auto csv = GetDoc(key);
    if (!csv)
        return " | << [CSV_NOT_FOUND: " + key + "]";
    try
    {
      return csv->GetCell<std::string>(language, dialog);
    }
    catch (const std::exception&)
    {
      return " | << [MISSING_TEXT: " + dialog + "/" + language + "]";
    }
  }

  std::vector<std::string> CSVManager::GetRangeText(const std::string& language, const std::string& from, const std::string& where, const std::string& key)
  {
    std::vector<std::string> dialogs;
    auto csv = GetDoc(key);
    if (!csv)
      return dialogs;

    try
    {
      auto fromIndx = csv->GetRowIdx(from);
      auto whereIndx = csv->GetRowIdx(where);
      dialogs.reserve(whereIndx - fromIndx + 1);

      for (size_t i = fromIndx; i <= whereIndx; ++i)
        dialogs.push_back(csv->GetCell<std::string>(language, i)); // <- texto, no el doc
    }
    catch (const std::exception&)
    {
      LOG_ERROR(" | << Range invalid, CSV: '" + key + "': " + from + " -> " + where);
    }
    return dialogs;
  }


}
