//// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Modules/LanguageRanges.hpp"
#include "Engine/Text/Font/Font.hpp"
// | -------------------------------
#include <filesystem>
#include <string>
#include <vector>
// | -------------------------------

namespace ENG
{ 
  class BakerFont
  {
    public:
      BakerFont(const std::string& fontPath);
      ~BakerFont() = default;
      
      bool Bake(const std::vector<CharRange>& ranges);
      bool Bake(const std::vector<Language>& languages);

      void SaveToDisk(const std::filesystem::path& outputDir, const std::string& fontName);
    private:
      std::vector<unsigned char> LoadFontFile(const std::filesystem::path& path);

    private:
      std::filesystem::path m_path;
      std::vector<unsigned char> m_buffer;
      std::vector<unsigned char> m_bitmap;
      std::vector<Glyph> m_glyphs;

      int m_bitmapWidth;
      int m_bitmapHeight;
      int m_lineHeight;
      float m_scale;
      bool m_valid = false;  };
}
