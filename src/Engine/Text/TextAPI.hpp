/// | -------------------------------
#pragma once
/// | -------------------------------
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | -------------------------------
#include <string>
/// | -------------------------------

namespace ENG
{
  class TextAPI
  {
    public:
      static TextAPI& Get(void);
      static void Destroy(void);

      std::string FormatText(const char* format, ...);
      void DrawText(const Font& font, float x, float y, float size, const std::string& text);
      void DrawTextEx(const Font& font, const Vector2& position, const std::string& text, float size, const Color& color);
      // void DrawTextPro(const Font& font, const Vector2& position, const std::string& text, float size, float rotation, const Color& color);

      float GetMeasureText(const Font* font, const std::string& text, const float fontSize = 0.0f);
      Vector2 GetMeasureTextEx(const Font* font, const std::string& text, const float fontSize = 0.0f);
    private:
      explicit TextAPI() = default;
      static inline TextAPI* m_textAPI = nullptr;
  };
}
