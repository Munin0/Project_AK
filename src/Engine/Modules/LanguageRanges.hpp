// | -------------------------------
#pragma once
// | -------------------------------
#include <vector>
#include <unordered_map>
// | -------------------------------

namespace ENG
{
  enum class Language
  {
    English,
    Spanish,
    French,
    German,
    Portuguese,
    Polish,
    Turkish,
    Russian,
    Greek,
  };

  struct CharRange
  {
    int firstCodepoint;
    int numChars;

    bool operator==(const CharRange& other) const
    {
      return firstCodepoint == other.firstCodepoint && numChars == other.numChars;
    }
  };

  inline const std::unordered_map<Language, std::vector<CharRange>> kLanguageRanges =
  {
    { Language::English,    { { 32, 95 } } },
    { Language::Spanish,    { { 32, 95 }, { 160, 96 } } },
    { Language::French,     { { 32, 95 }, { 160, 96 } } },
    { Language::German,     { { 32, 95 }, { 160, 96 } } },
    { Language::Portuguese, { { 32, 95 }, { 160, 96 } } },
    { Language::Polish,     { { 32, 95 }, { 256, 128 } } },
    { Language::Turkish,    { { 32, 95 }, { 256, 128 } } },
    { Language::Russian,    { { 32, 95 }, { 1024, 256 } } },
    { Language::Greek,      { { 32, 95 }, { 880, 144 } } },
  };

  inline std::vector<CharRange> GetRangesForLanguages(const std::vector<Language>& languages)
  {
    std::vector<CharRange> result;
    for (const auto& lang : languages)
    {
      auto it = kLanguageRanges.find(lang);
      if (it == kLanguageRanges.end())
        continue;
      for (const auto& range : it->second)
      {
        bool alreadyExists = false;
        for (const auto& existing : result)
        {
          if (existing == range)
          {
            alreadyExists = true;
            break;
          }
        }
        if (!alreadyExists)
          result.push_back(range);
      }
    }
    return result;
  }

}
