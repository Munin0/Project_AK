/// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  /**
   * @brief UI text box: a rectangle that shows a text the user can edit.
   *
   * A TextBox is an Object that draws a rectangle (with a background color
   * and an outline) and a text inside it. The text typed in the box is kept
   * in an internal storage that can be read with GetTextStorage().
   *
   * Usage:
   * @code
   * ENG::TextBox nameBox("name_box", "", {300.0f, 40.0f});
   * nameBox.SetPosition({100.0f, 100.0f});
   * nameBox.SetTextColor(ENG::Color::White);
   * @endcode
   */
  class TextBox : public Object
  {
    public:
      /// @name Constructor and destructor
      /// @{

      /**
       * @brief Creates a text box.
       *
       * @param[in] name       Name of the text box.
       * @param[in] text       Initial text.
       * @param[in] dimention  Size of the text box: { width, height }.
       */
      TextBox(const std::string& name, const std::string& text, const Vector2& dimention);

      /// @brief Default destructor.
      ~TextBox() override {};
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
       * @brief Updates the text box once per frame.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      void Update(float dt) override;
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
       * @param[in] dimention New size: { width, height }.
       */
      void SetDimention(const Vector2& dimention);
      /// @}

      /// @name Font configuration
      /// @{

      /**
       * @brief Sets the text of the text box.
       *
       * @param[in] text New text.
       */
      void SetText(const std::string& text);

      /**
       * @brief Sets the font size of the text.
       *
       * @param[in] size New font size.
       */
      void SetFontSize(float size);

      /**
       * @brief Sets the color of the text.
       *
       * @param[in] color New text color.
       */
      void SetTextColor(const Color& color);

      /**
       * @brief Sets the font of the text.
       *
       * @param[in] font Font to use.
       */
      void SetFont(Font& font);

      /**
       * @brief Returns the text stored in the text box.
       *
       * @return The text held by the text box, without any decoration
       *         added when it is displayed.
       */
      std::string GetTextStorage(void) const;
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

      /// @name Extra configuration
      /// @{

      /**
       * @brief Sets the delimiter character of the text box.
       *
       * @param[in] delimiter New delimiter character. It is '|' by default.
       */
      void SetDelimiter(const char& delimiter);
      /// @}

    private:
      /// @brief Delimiter character. Defaults to '|'.
      char m_crDelimiter = '|';

      /// @brief Text stored in the text box.
      std::string m_textStorage;

      /// @brief Text as it is displayed.
      IText m_text;

      /// @brief Position where the text is drawn.
      Vector2 m_textPosition;

      /// @brief Color of the text.
      IColor m_textColor;

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

      /// @brief Whether the text box is currently pressed.
      bool m_isPressed = false;
  };
}
