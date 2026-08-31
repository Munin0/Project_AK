/// | -------------------------------
#pragma once
/// | -------------------------------
#include "Engine/Render/Image/RImage.hpp"
/// | -------------------------------
#include "nlohmann/json_fwd.hpp"
/// | -------------------------------
#include <memory>
#include <string>
#include <unordered_map>
/// | -------------------------------

namespace ENG
{
  struct Glyph
  {
    int codepoint;
    float u0, v0, u1, v1;
    float xoff, yoff;
    float xoff2, yoff2;
    float xadvance;
  };

  class Font
  {
    public:
      Font(const std::string& fontName);
      ~Font() = default;

      std::string GetName(void) const {return this->m_fontName;} 
      std::shared_ptr<RImage> GetTexture()const {return this->m_texture;}
      float GetLineHeight()const {return this->m_lineHeight;}
      const Glyph* GetGlyphs(int c)const;
    private:
      bool LoadFromDisk(const std::string& fontName);
      bool ParseFontJSON(const nlohmann::json& j);
    private:
      std::string m_fontName;
      std::shared_ptr<RImage> m_texture;
      float m_lineHeight;
      std::unordered_map<int, Glyph> m_glyphs;
  };
}
