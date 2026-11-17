// | -------------------------------
#include "TextAPI.hpp"
// | -------------------------------
#include "Engine/Render/Color/Color.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Text/Utf8.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include "glm/ext/vector_float2.hpp"
// | -------------------------------
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <string>
// | -------------------------------

namespace ENG
{
  TextAPI& TextAPI::Get(void)
  {
    if(!m_textAPI)
      m_textAPI = new TextAPI();
    return *m_textAPI;
  }

  void TextAPI::Destroy(void)
  {
    delete m_textAPI;
  }

  std::string TextAPI::FormatText(const char* format, ...)
  {
    va_list args;

    va_start(args, format);
    int size = std::vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (size <= 0)
        return {};
    std::string result(size, '\0');

    va_start(args, format);
    std::vsnprintf(result.data(), size + 1, format, args);
    va_end(args);

    return result;
  }

  void TextAPI::DrawText(const Font& font, float x, float y, float size, const std::string& text)
  {
    auto& b = Render::Get().GetBatcher();
    const auto tx = font.GetTexture();
    const Vector2 pos = {x,y};
    float scaleFactor = size / font.GetLineHeight();

    glm::vec2 cursor = pos;
    
    for(unsigned char c : text)
    {
      if(c == '\n')
      {
        cursor.x = pos.x;
        cursor.y -= font.GetLineHeight();
        continue;
      }
        
      const Glyph* g = font.GetGlyphs((int)c);
      if(!g) continue;
      
      glm::vec2 quadPos  = cursor + glm::vec2(g->xoff, g->yoff) * scaleFactor;
      glm::vec2 quadSize = glm::vec2(g->xoff2 - g->xoff, g->yoff2 - g->yoff) * scaleFactor;

      b.DrawGlyph(quadPos, quadSize, tx, {g->u0, g->v0}, {g->u1, g->v1}, Color::White);

      cursor.x += g->xadvance * scaleFactor;
    }
  }

  void TextAPI::DrawTextEx(const Font& font, const Vector2& position, const std::string& text, float size, const Color& color)
  {
    auto& b = Render::Get().GetBatcher();
    const auto tx = font.GetTexture();
    float scaleFactor = size / font.GetLineHeight();

    glm::vec2 cursor = position;
    
    // UTF-8: one glyph per codepoint ("ó" is two bytes but one glyph).
    for(size_t i = 0; i < text.size();)
    {
      const uint32_t c = Utf8::Next(text, i);
      if(c == '\n')
      {
        cursor.x = position.x;
        cursor.y += font.GetLineHeight() * scaleFactor;
        continue;
      }

      const Glyph* g = font.GetGlyphs(static_cast<int>(c));
      if(!g) continue;
      
      glm::vec2 quadPos  = cursor + glm::vec2(g->xoff, g->yoff) * scaleFactor;
      glm::vec2 quadSize = glm::vec2(g->xoff2 - g->xoff, g->yoff2 - g->yoff) * scaleFactor;

      b.DrawGlyph(quadPos, quadSize, tx, {g->u0, g->v0}, {g->u1, g->v1}, color);

      cursor.x += g->xadvance * scaleFactor;
    }
  }

  float TextAPI::GetMeasureText(const Font* font, const std::string& text, const float fontSize)
  {
    // Same width as GetMeasureTextEx().x
    return GetMeasureTextEx(font, text, fontSize).x;
  }

  // Return Vector2, X = FontSize Width, Y return scaleRender if the size of the font is > of GetLineHeight().
  // GetLineHeight, gets the default size of he font*.
  Vector2 TextAPI::GetMeasureTextEx(const Font* font, const std::string& text, const float fontSize)
  {
    float scale = fontSize != 0 ? fontSize / font->GetLineHeight() : 1.0f;
    float measure = 0.0f;
    // UTF-8 codepoints; glyphs missing in the font are skipped (as DrawTextEx does).
    for(size_t i = 0; i < text.size();)
    {
      if(const Glyph* g = font->GetGlyphs(static_cast<int>(Utf8::Next(text, i))))
        measure += g->xadvance * scale;
    }
    float height = fontSize != 0 ? fontSize : font->GetLineHeight();
    return {measure, height};
  }

}
