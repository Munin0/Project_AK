// | -------------------------------
#include "TextAPI.hpp"
// | -------------------------------
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include "glm/ext/vector_float2.hpp"
// | -------------------------------
#include <cstdarg>
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
    
    for(unsigned char c : text)
    {
      if(c == '\n')
      {
        cursor.x = position.x;
        cursor.y += font.GetLineHeight();
        continue;
      }
        
      const Glyph* g = font.GetGlyphs((int)c);
      if(!g) continue;
      
      glm::vec2 quadPos  = cursor + glm::vec2(g->xoff, g->yoff) * scaleFactor;
      glm::vec2 quadSize = glm::vec2(g->xoff2 - g->xoff, g->yoff2 - g->yoff) * scaleFactor;

      b.DrawGlyph(quadPos, quadSize, tx, {g->u0, g->v0}, {g->u1, g->v1}, color);

      cursor.x += g->xadvance * scaleFactor;
    }
  }

  float TextAPI::GetMeasureText(const Font* font, const std::string& text, const float fontSize)
  {
    float measure = 0.0f;
    float scaleRender = 1.0f;
    if(fontSize != 0)
    {
      float scaleWidth = fontSize / font->GetLineHeight();
      scaleRender = font->GetLineHeight() * scaleWidth; 
    }
    for(auto& c : text)
    {
      measure += font->GetGlyphs(c)->xadvance; 
    }
    return measure * scaleRender;
  }

  // Return Vector2, X = FontSize Width, Y return scaleRender if the size of the font is > of GetLineHeight().
  // GetLineHeight, gets the default size of he font*.
  Vector2 TextAPI::GetMeasureTextEx(const Font* font, const std::string& text, const float fontSize)
  {
    float scale = fontSize != 0 ? fontSize / font->GetLineHeight() : 1.0f;
    float measure = 0.0f;
    for(auto& c : text)
    {
      measure += font->GetGlyphs(c)->xadvance * scale;
    }
    float height = fontSize != 0 ? fontSize : font->GetLineHeight();
    return {measure, height};
  }

}
