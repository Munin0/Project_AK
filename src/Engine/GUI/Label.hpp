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
   * @brief Position of the text inside a Label.
   */
  enum class AlignText
  {
    TEXT_ALIGN_UPL    = 0,  ///< Upper-left corner of the label.
    TEXT_ALIGN_UPR    = 1,  ///< Upper-right corner of the label.
    TEXT_ALIGN_DOWNL  = 2,  ///< Lower-left corner of the label.
    TEXT_ALIGN_DOWNR  = 3,  ///< Lower-right corner of the label.
    TEXT_ALIGN_CENTER = 4   ///< Center of the label.
  };

  /**
   * @brief UI label: a text drawn inside a rectangle.
   *
   * A Label is an Object that draws a rectangle (with a background color
   * and, optionally, a texture) and a text inside it. The text can be
   * aligned to a corner or to the center of the label and moved with an
   * offset.
   *
   * Usage:
   * @code
   * ENG::Label title("title", {300.0f, 40.0f});
   * title.SetText("Score: 0");
   * title.SetTextAlign(ENG::AlignText::TEXT_ALIGN_CENTER);
   * @endcode
   */
  class Label : public Object
  {
    public:
      /**
       * @brief Creates a label.
       *
       * @param[in] name       Name of the label.
       * @param[in] dimention  Size of the label: { width, height }.
       */
      Label(const std::string& name, const Vector2& dimention);

      /// @brief Default destructor.
      ~Label() override {};

      /**
       * @brief Submits the label to the Batcher for rendering.
       *
       * @param[in,out] b Batcher that collects the draw data of this frame.
       */
      void Draw(Batcher& b) const override;

      /**
       * @brief Updates the label once per frame.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      void Update(float dt) override;

      /**
       * @brief Sets the text of the label.
       *
       * @param[in] text New text.
       */
      void SetText(const std::string& text);

      /**
       * @brief Sets the font of the text.
       *
       * @param[in] font Font to use.
       */
      void SetFont(Font& font);

      /**
       * @brief Sets the color of the text.
       *
       * @param[in] color New text color.
       */
      void SetColorText(const Color& color);

      /**
       * @brief Sets the font size of the text.
       *
       * @param[in] size New font size.
       */
      void SetFontSize(float size);

      /**
       * @brief Sets an offset applied to the position of the text.
       *
       * @param[in] offset Offset from the aligned position of the text.
       */
      void SetOffset(const Vector2& offset);

      /**
       * @brief Sets the background color of the label.
       *
       * @param[in] bgColor New background color.
       */
      void SetBackgoundColor(const Color& bgColor);

      /**
       * @brief Sets a texture for the background of the label.
       *
       * @param[in] key      Key of the atlas that contains the texture.
       * @param[in] textName Name of the texture (tile) inside the atlas.
       */
      void SetTexture(const std::string& key, const std::string& textName);

      /**
       * @brief Sets where the text is placed inside the label.
       *
       * @param[in] position Alignment of the text.
       */
      void SetTextAlign(AlignText position);

    private:
      /// @brief Text of the label.
      IText m_text;

      /// @brief Color of the text.
      IColor m_colorText;

      /// @brief Position where the text is drawn.
      Vector2 m_fontPosition;

      /// @brief Bounding box of the label, stored in the object's list of
      ///        boxes. Not owned.
      const IBoundingBox* m_box;

      /// @brief Background color.
      IColor m_bgColor;

      /// @brief Outline color.
      IColor m_outlineColor;

      /// @brief Atlas of the label's texture. Not owned. nullptr if the
      ///        label has no texture.
      const AtlasData* m_sprite = nullptr;

      /// @brief UV rectangle of the label's texture inside the atlas.
      UVRect m_textureRect;

      /// @brief Offset applied to the position of the text.
      Vector2 m_offset;
  };
}
