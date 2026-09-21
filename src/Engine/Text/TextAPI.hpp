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
  /**
   * @brief Text rendering and text measurement API.
   *
   * TextAPI is a singleton that groups every text-related operation of the
   * engine: string formatting, drawing text on screen and measuring text
   * so it can be laid out (centering, alignment, UI sizing, etc.).
   *
   * Access the instance through TextAPI::Get() and release it with
   * TextAPI::Destroy() during engine shutdown.
   *
   * @note The class is not copyable through public means: the constructor is
   *       private, so the only valid instance is the one returned by Get().
   */
  class TextAPI
  {
    public:
      /**
       * @brief Returns the global TextAPI instance.
       *
       * The instance is created on the first call and reused afterwards.
       *
       * @return Reference to the singleton instance.
       */
      static TextAPI& Get(void);

      /**
       * @brief Destroys the global TextAPI instance.
       *
       * Must be called once, at engine shutdown. Any reference previously
       * obtained through Get() becomes invalid after this call.
       */
      static void Destroy(void);

      /**
       * @brief Builds a std::string from a printf-style format string.
       *
       * @param[in] format printf-style format string (e.g. "FPS: %d").
       * @param[in] ...    Arguments matching the format specifiers.
       * @return The formatted text as a std::string.
       *
       * @warning The arguments are not type-checked at compile time. A
       *          mismatch between the specifiers and the arguments results
       *          in undefined behavior.
       */
      std::string FormatText(const char* format, ...);

      /**
       * @brief Draws text on screen at the given coordinates.
       *
       * Simplified overload: it does not take a color. Use DrawTextEx() if
       * you need to control the text color.
       *
       * @param[in] font Font used to render the text.
       * @param[in] x    Horizontal position of the text, in pixels.
       * @param[in] y    Vertical position of the text, in pixels.
       * @param[in] size Font size to render with.
       * @param[in] text Text to draw.
       */
      void DrawText(const Font& font, float x, float y, float size, const std::string& text);

      /**
       * @brief Draws colored text on screen at the given position.
       *
       * Extended version of DrawText() that takes the position as a Vector2
       * and allows tinting the text with a color.
       *
       * @param[in] font     Font used to render the text.
       * @param[in] position Position of the text, in pixels.
       * @param[in] text     Text to draw.
       * @param[in] size     Font size to render with.
       * @param[in] color    Color used to tint the text.
       */
      void DrawTextEx(const Font& font, const Vector2& position, const std::string& text, float size, const Color& color);

      // TODO: Not available yet. Planned version of DrawTextEx() with rotation support.
      //void DrawTextPro(const Font& font, const Vector2& position, const std::string& text, float size, float rotation, const Color& color);

      /**
       * @brief Measures the width of a text string.
       *
       * @param[in] font     Font used to measure the text.
       * @param[in] text     Text to measure.
       * @param[in] fontSize Size to measure with. When 0.0f (default), the
       *                     font's own default size is used.
       * @return Width of the text, in pixels.
       *
       * @see GetMeasureTextEx() to get both width and height.
       */
      float GetMeasureText(const Font* font, const std::string& text, const float fontSize = 0.0f);

      /**
       * @brief Measures the full dimensions of a text string.
       *
       * @param[in] font     Font used to measure the text.
       * @param[in] text     Text to measure.
       * @param[in] fontSize Size to measure with. When 0.0f (default), the
       *                     font's own default size is used.
       * @return A Vector2 where x is the width and y is the height of the
       *         text, in pixels.
       *
       * @see GetMeasureText() if only the width is needed.
       */
      Vector2 GetMeasureTextEx(const Font* font, const std::string& text, const float fontSize = 0.0f);

    private:
      /// @brief Private constructor; instances are managed through Get() and Destroy().
      explicit TextAPI() = default;

      /// @brief Pointer to the global instance. nullptr while no instance exists.
      static inline TextAPI* m_textAPI = nullptr;
  };
}
