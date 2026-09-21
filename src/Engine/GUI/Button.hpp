// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Geometry/RGeometry.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <any>
#include <functional>
#include <string>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Clickable UI button.
   *
   * A Button is an Object that draws a rectangle (with a background color,
   * an outline, and optionally a texture) and a text label, and runs a
   * callback when it is activated. Any value can be attached to the button
   * with SetData() and given to the callback.
   *
   * Usage:
   * @code
   * ENG::Button play("play", {200.0f, 50.0f});
   * play.SetText("Play");
   * play.SetBackgroundColor(ENG::Color::Blue);
   * play.SetFunction([](std::any) { StartGame(); });
   * @endcode
   */
  class Button : public Object
  {
    public:
      /// @brief Type of the function called when the button is activated. It
      ///        receives a value of any type (see SetData()).
      using ButtonCallback = std::function<void(std::any)>;

      /**
       * @brief Creates a button without a callback.
       *
       * @param[in] _name      Name of the button.
       * @param[in] dimention  Size of the button: { width, height }.
       */
      Button(const std::string& _name, const Vector2& dimention);

      /**
       * @brief Creates a button with a callback.
       *
       * @param[in] _name      Name of the button.
       * @param[in] dimention  Size of the button: { width, height }.
       * @param[in] _func      Function called when the button is activated.
       *                       It receives a value of any type and returns an int.
       */
      Button(const std::string& _name, const Vector2& dimention, std::function<int(std::any)> _func);

      /// @brief Default destructor.
      ~Button() override {};

      /**
       * @brief Updates the button once per frame.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      void Update(float dt) override;

      /**
       * @brief Submits the button to the Batcher for rendering.
       *
       * @param[in,out] b Batcher that collects the draw data of this frame.
       */
      void Draw(Batcher& b) const override;

      /**
       * @brief Runs the callback of the button.
       *
       * @param[in] _ Value given to the callback.
       */
      void Action(std::any _);

      /**
       * @brief Sets a texture for the button.
       *
       * @param[in] atlas    Key of the atlas that contains the texture.
       * @param[in] textName Name of the texture (tile) inside the atlas.
       */
      void SetTexture(const std::string& atlas, const std::string& textName);

      /**
       * @brief Sets the background color.
       *
       * @param[in] color New background color.
       */
      void SetBackgroundColor(const Color& color);

      /**
       * @brief Sets the outline color.
       *
       * @param[in] color New outline color.
       */
      void SetOutlineColor(const Color& color);

      /**
       * @brief Sets the function called when the button is activated.
       *
       * @param[in] func New callback.
       */
      void SetFunction(ButtonCallback func);

      /**
       * @brief Attaches a value of any type to the button.
       *
       * @param[in] _ Value to store.
       */
      void SetData(std::any _);

      /**
       * @brief Sets the text of the label.
       *
       * @param[in] text New text.
       */
      void SetText(const std::string& text);

      /**
       * @brief Sets the font size of the label.
       *
       * @param[in] size New font size.
       */
      void SetTextSize(float size);

      /**
       * @brief Sets the color of the label.
       *
       * @param[in] color New text color.
       */
      void SetTextColor(const Color& color);

      /**
       * @brief Sets the font of the label.
       *
       * @param[in] font Font to use.
       */
      void SetFont(Font& font);

      /**
       * @brief Returns the value attached to the button.
       * @return Reference to the stored value.
       */
      std::any& GetData(void);

      /**
       * @brief Returns the value attached to the button.
       * @return Constant reference to the stored value.
       */
      const std::any& GetData(void) const;

      /**
       * @brief Returns the rectangle of the button.
       * @return Rectangle with the position { x, y } and the size { w, h } of the button.
       */
      Rectangle GetRect(void);

    private:
      /// @brief Function called when the button is activated. nullptr if none.
      ButtonCallback m_callback = nullptr;

      /// @brief Value attached to the button.
      std::any m_data;

      /// @brief Text of the label.
      IText m_text;

      /// @brief Color of the label.
      IColor m_fontColor;

      /// @brief Background color.
      IColor m_bgColor;

      /// @brief Outline color.
      IColor m_outlineColor;

      /// @brief Bounding box of the button, stored in the object's list of
      ///        boxes. Not owned. nullptr until created.
      IBoundingBox* m_box = nullptr;

      /// @brief Atlas of the button's texture. Not owned. nullptr if the
      ///        button has no texture.
      const AtlasData* m_sprite = nullptr;

      /// @brief UV rectangle of the button's texture inside the atlas.
      UVRect m_textureRect{};

      /// @brief Whether the button has a texture.
      bool m_hasTexture = false;

      /// @brief Whether the button is currently pressed.
      bool m_pressed = false;
  };
}
