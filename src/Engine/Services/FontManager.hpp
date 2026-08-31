// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Text/Font/Font.hpp"
// | -------------------------------
#include <memory>
#include <string>
#include <unordered_map>
// | -------------------------------

namespace ENG
{
  class FontManager
  {
    public:
      FontManager() = default;
      ~FontManager() = default;

      void LoadFont(std::unique_ptr<Font> font);
      Font* GetFont(const std::string& fontName);
      
      void Clear(void);
    private:
      std::unordered_map<std::string, std::unique_ptr<Font>> m_fonts;
  };
}
