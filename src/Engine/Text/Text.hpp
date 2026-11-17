/// | -------------------------------
#pragma once
/// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | -------------------------------
#include <string>
/// | -------------------------------

namespace ENG
{
  /**
   * @brief Scene object that displays a piece of text.
   *
   * Text is an Object whose text data (content, font size and font) lives in
   * an IText component, like any other component. Because of that it has no
   * Save()/Load() of its own: Object::Save() and Object::Load() already store
   * and restore the IText (as ObjectState::m_txData), together with the
   * transform, the IColor and the rest of the components.
   *
   * @code
   * auto txt = std::make_unique<ENG::Text>(ENG::Vector2{100, 100}, 32.0f, "Hello");
   * txt->SetFont(ENG::Services::Fonts().GetFont("Roboto"));
   * txt->AddComponent<ENG::IColor>(ENG::Color::Black);
   * @endcode
   *
   * @note The font is restored on Load() by its key (Font::GetName()), so the
   *       font must be loaded in the FontManager before loading the object.
   */
  class Text : public Object
  {
    public:
      /**
       * @brief Creates a text object.
       *
       * @param[in] position Position of the text in the scene.
       * @param[in] fontSize Font size used to render the text.
       * @param[in] text     Initial text content. It is also used as the object name.
       */
      Text(const Vector2& position, float fontSize, const std::string& text);

      /// @brief Default destructor.
      ~Text() override = default;

      /**
       * @brief Submits the text for rendering.
       *
       * Does nothing if the text has no font. Uses the IColor component as the
       * text color, or white if the object has none.
       *
       * @param[in,out] b Batcher that collects the draw data of this frame.
       */
      void Draw(Batcher& b) const override;

      /**
       * @brief Updates the text object once per frame.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      void Update(float dt) override;

      /**
       * @brief Replaces the displayed text.
       *
       * @param[in] newText New text content.
       */
      void ChangeText(const std::string& newText);

      /**
       * @brief Changes the font size used to render the text.
       *
       * @param[in] newFontSize New font size.
       */
      void ChangeSizeFont(float newFontSize);

      /**
       * @brief Sets the font used to render the text. Mandatory before drawing.
       *
       * @param[in] font Font to use. Not owned: it must outlive this object
       *                 (fonts are owned by the FontManager).
       */
      void SetFont(Font* font);

      /// @brief Returns the displayed text.
      std::string GetText() const;

      /// @brief Returns the font size.
      float GetFontSize() const;

    private:
      /**
       * @brief Returns the IText component of this object.
       *
       * The pointer is looked up every time instead of being cached, because
       * Object::Load() replaces the component and would leave a cached
       * pointer dangling.
       */
      IText* TextData() const;
  };
}
