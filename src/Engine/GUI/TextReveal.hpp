// | -------------------------------
#pragma once
// | -------------------------------
#include <cstddef>
#include <string>
#include <string_view>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Reveals a text little by little ("typewriter" effect).
   *
   * It only counts how much of the text is visible; it does not draw. Any
   * widget can use it (Label does). It works by UTF-8 codepoints, so "¿Cómo?"
   * is never cut in the middle of a character.
   *
   * @code
   * ENG::TextReveal reveal;
   * reveal.Start("¡Hola, viajero!", 30.0f);   // 30 characters per second
   * // every frame:
   * reveal.Update(dt);
   * DrawText(reveal.GetVisible());
   * if (pressedAction) reveal.IsDone() ? NextLine() : reveal.Finish();
   * @endcode
   */
  class TextReveal
  {
    public:
      /**
       * @brief Starts revealing @p text from the beginning.
       *
       * @param[in] text           UTF-8 text.
       * @param[in] charsPerSecond Speed. <= 0 shows the whole text at once.
       */
      void Start(std::string text, float charsPerSecond);

      /// @brief Shows @p text completely, with no effect.
      void Show(std::string text);

      /// @brief Advances the effect. Does nothing when it is done.
      void Update(float dt);

      /// @brief Shows the rest of the text at once.
      void Finish();

      /// @brief Whether the whole text is visible.
      bool IsDone() const { return m_visibleChars >= m_totalChars; }

      /// @brief Visible part of the text (a view into GetText(); valid until the text changes).
      std::string_view GetVisible() const;

      /// @brief The whole text.
      const std::string& GetText() const { return m_text; }

    private:
      std::string m_text;
      size_t      m_totalChars   = 0;     ///< Codepoints of m_text.
      size_t      m_visibleChars = 0;     ///< Codepoints visible.
      size_t      m_visibleBytes = 0;     ///< Bytes of m_text visible (cached).
      float       m_progress     = 0.0f;  ///< Fractional visible chars.
      float       m_speed        = 0.0f;  ///< Characters per second.
  };
}
