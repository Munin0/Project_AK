/// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Color/Color.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <cstddef>
#include <functional>
#include <string>
// | -------------------------------

namespace ENG
{
  /**
   * @brief UI text box: a rectangle where the user types a text.
   *
   * - Focus: clicking the box (OnPointer()) gives it the focus and consumes the
   *   click; any other click, even one taken by another object, removes it.
   *   Enter and Escape remove it too.
   * - Typing: while focused, the box owns the text input of PollEvent
   *   (StartTextInput), so the text comes composed by SDL (keyboard layout,
   *   accents, ñ, capitals), in UTF-8, and the keyboard is captured: the game
   *   does not see the keys.
   * - Editing: a caret marks where the text goes. Left/Right move it one
   *   character, Home/End to the start/end; Backspace deletes the character
   *   before it, Delete the one after it. All of them repeat while held and
   *   work by character (UTF-8), not by byte. Focusing puts the caret at the end.
   * - Selection: Ctrl+A selects all the text (highlighted). With a selection,
   *   typing replaces it, Backspace/Delete remove it, Left/Right/Home/End
   *   cancel it (Left/Right leave the caret at its start/end).
   * - Enter calls the submit callback (SetOnSubmit()) with the text.
   * - Empty and unfocused, it shows the placeholder (gray). Focused, a
   *   blinking caret (the delimiter, '|') is drawn at its position. A text wider
   *   than the box scrolls so the caret is always visible.
   *
   * Only the characters of the font atlas can be drawn (CabinItalic: 32..255,
   * Latin-1); others are stored but skipped when drawing.
   *
   * Usage:
   * @code
   * auto box = std::make_unique<ENG::TextBox>("name_box", "Ingresa nombre", ENG::Vector2{300, 40});
   * box->SetFont(*font);
   * box->SetPosition({100.0f, 100.0f});
   * box->SetMaxLength(16);
   * box->SetOnSubmit([](const std::string& name) { SavePlayerName(name); });
   * m_pool.Add(std::move(box));
   * // Inputs(): ENG::DispatchPointer(m_pool);   Update(): ENG::UpdateObjects(m_pool, dt);
   * @endcode
   */
  class TextBox : public Object
  {
    public:
      /// @brief Function called when Enter is pressed; receives the typed text.
      using SubmitCallback = std::function<void(const std::string&)>;

      /// @name Constructor and destructor
      /// @{

      /**
       * @brief Creates a text box.
       *
       * @param[in] name        Name of the text box.
       * @param[in] placeholder Hint shown while the box is empty (e.g. "Enter name").
       * @param[in] dimention   Size of the text box: { width, height }.
       */
      TextBox(const std::string& name, const std::string& placeholder, const Vector2& dimention);

      /// @brief Releases the text input if the box still has the focus.
      ~TextBox() override;
      /// @}

      /// @name Inheritance of Object
      /// @{

      /**
       * @brief Submits the text box to the Batcher for rendering.
       *
       * @param[in,out] b Batcher that collects the draw data of this frame.
       */
      void Draw(Batcher& b) const override;

      /**
       * @brief Updates the layout and, while focused, applies the typed text and keys.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      void Update(float dt) override;

      /**
       * @brief Takes the focus when clicked, and loses it on a click elsewhere.
       *
       * @param[in] mouse Mouse position in the text box's RenderSpace.
       */
      void OnPointer(const Vector2& mouse) override;
      /// @}

      /// @name Transform and dimension
      /// @{

      /**
       * @brief Sets the position of the text box.
       *
       * @param[in] position New position.
       */
      void SetPosition(const Vector2& position);

      /**
       * @brief Sets the size of the text box.
       *
       * @param[in] dimension New size: { width, height }.
       */
      void SetDimension(const Vector2& dimension);
      /// @}

      /// @name Text
      /// @{

      /**
       * @brief Replaces the typed text (cut to the maximum length).
       *
       * @param[in] text New text, UTF-8.
       */
      void SetText(const std::string& text);

      /**
       * @brief Sets the hint shown while the box is empty and unfocused.
       *
       * @param[in] placeholder New hint.
       */
      void SetPlaceholder(const std::string& placeholder);

      /**
       * @brief Maximum number of characters (codepoints, not bytes).
       *
       * @param[in] maxLength New maximum. 0 means no limit. Defaults to 32.
       */
      void SetMaxLength(size_t maxLength);

      /**
       * @brief Sets the function called when Enter is pressed.
       *
       * @param[in] callback Receives the typed text. The box loses the focus after it.
       */
      void SetOnSubmit(SubmitCallback callback);

      /**
       * @brief Returns the typed text.
       *
       * @return The text held by the text box (UTF-8), without the placeholder
       *         or the caret.
       */
      std::string GetTextStorage(void) const;
      /// @}

      /// @name Font configuration
      /// @{

      /**
       * @brief Sets the font size of the text.
       *
       * @param[in] size New font size.
       */
      void SetFontSize(float size);

      /**
       * @brief Sets the font of the text.
       *
       * @param[in] font Font to use.
       */
      void SetFont(Font& font);

      /**
       * @brief Sets the color of the typed text.
       *
       * @param[in] color New text color.
       */
      void SetTextColor(const Color& color);

      /**
       * @brief Sets the color of the selection highlight.
       *
       * @param[in] color New highlight color. Light blue by default.
       */
      void SetSelectionColor(const Color& color);

      /**
       * @brief Sets the color of the placeholder.
       *
       * @param[in] color New placeholder color. Gray by default.
       */
      void SetPlaceholderColor(const Color& color);
      /// @}

      /// @name Rectangle colors
      /// @{

      /**
       * @brief Sets the background color of the rectangle.
       *
       * @param[in] color New background color.
       */
      void SetBackgroundColor(const Color& color);

      /**
       * @brief Sets the outline color of the rectangle.
       *
       * @param[in] color New outline color.
       */
      void SetOutlineColor(const Color& color);
      /// @}

      /// @name Focus and extra configuration
      /// @{

      /**
       * @brief Sets the caret character of the text box.
       *
       * @param[in] delimiter New caret character. It is '|' by default.
       */
      void SetDelimiter(const char& delimiter);

      /**
       * @brief Gives or removes the focus from code (same as clicking it / elsewhere).
       *
       * @param[in] _ true to focus the box, false to unfocus it.
       */
      void SetInteraction(bool _);

      /// @brief Whether the text box has the focus (it is receiving text).
      bool IsFocused(void) const { return m_isPressed; }
      /// @}

    private:
      /// @brief Takes the focus and the text input of PollEvent.
      void Focus(void);

      /// @brief Drops the focus and releases the text input.
      void Unfocus(void);

      /// @brief Inserts @p text at the caret, codepoint by codepoint, up to the
      ///        maximum length, and moves the caret after it.
      void Insert(const std::string& text);

      /// @brief Selects all the text (Ctrl+A): anchor at the start, caret at the end.
      void SelectAll(void);

      /// @brief Whether there is a selection (anchor and caret differ).
      bool HasSelection(void) const { return m_anchor != m_caret; }

      /// @brief Removes the selected text and leaves the caret where it was.
      void DeleteSelection(void);

      /// @brief Moves the caret to @p pos and drops the selection.
      void MoveCaret(size_t pos);

      /// @brief Moves m_scroll so the caret is inside the visible width of the box.
      void UpdateScroll(void);

      /// @brief Width available for the text inside the box (minus padding and caret).
      float VisibleWidth(void) const;

      /// @brief Caret character. Defaults to '|'.
      char m_crDelimiter = '|';

      /// @brief Typed text, UTF-8.
      std::string m_textStorage = "";

      /// @brief Caret position: byte offset in m_textStorage, always on a
      ///        codepoint boundary (0 = before the first character).
      size_t m_caret = 0;

      /// @brief Other end of the selection (byte offset). Equal to m_caret when
      ///        nothing is selected; the selection is [min, max) of both.
      size_t m_anchor = 0;

      /// @brief First visible character: byte offset in m_textStorage, for
      ///        texts wider than the box. Kept by UpdateScroll().
      size_t m_scroll = 0;

      /// @brief Hint shown while the box is empty and unfocused.
      std::string m_placeholder = "";

      /// @brief Font and size of the text (its own text is not used).
      IText m_text;

      /// @brief Position where the text is drawn (left, baseline).
      Vector2 m_textPosition;

      /// @brief Color of the typed text.
      IColor m_textColor;

      /// @brief Color of the placeholder.
      IColor m_placeholderColor;

      /// @brief Color of the selection highlight.
      IColor m_selectionColor;

      /// @brief Background color.
      IColor m_bgColor;

      /// @brief Outline color.
      IColor m_outlineColor;

      /// @brief Bounding box of the text box, stored in the object's list of
      ///        boxes. Not owned. nullptr until created.
      IBoundingBox* m_box = nullptr;

      /// @brief Atlas of the text box's texture. Not owned. nullptr if the
      ///        text box has no texture.
      const AtlasData* m_sprite = nullptr;

      /// @brief UV rectangle of the text box's texture inside the atlas.
      UVRect m_textureRect{};

      /// @brief Whether the text box has a texture.
      bool m_hasTexture = false;

      /// @brief Whether the text box has the focus. Set by Focus()/Unfocus().
      bool m_isPressed = false;

      /// @brief Whether the mouse is over the text box. Updated by OnPointer().
      bool m_hover = false;

      /// @brief Whether the text position is up to date (recomputed in Update()).
      bool m_ConfigComplete = false;

      /// @brief Maximum number of characters. 0 = no limit.
      size_t m_maxLength = 128;

      /// @brief Time since the focus was taken or the text changed, for the caret blink.
      float m_caretTimer = 0.0f;

      /// @brief Called on Enter with the text. nullptr if none.
      SubmitCallback m_onSubmit = nullptr;
  };
}
