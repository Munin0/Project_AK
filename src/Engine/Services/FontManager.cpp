// | -------------------------------
#include "FontManager.hpp"
// | -------------------------------
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <memory>
#include <string>
#include <utility>
// | -------------------------------

namespace ENG
{
  void FontManager::Clear(void)
  {
    m_fonts.clear();
  }

  void FontManager::LoadFont(std::unique_ptr<Font> font)
  {
    if(m_fonts.contains(font->GetName()))
    {
      LOG_ERROR(" | << ERROR: font exits " + font->GetName());
      return;
    }
    m_fonts.emplace(font->GetName(),std::move(font));
  }

  Font* FontManager::GetFont(const std::string& fontName)
  {
    return m_fonts.find(fontName)->second != nullptr ? m_fonts.at(fontName).get() : nullptr;
  }
}
