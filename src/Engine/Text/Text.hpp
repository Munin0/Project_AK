/// | -------------------------------
#pragma once
/// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Services/WorldSaver.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | -------------------------------
/// | -------------------------------
#include <string>
/// | -------------------------------

namespace ENG
{
  /**
   * @brief Scene object that displays a piece of text.
   *
   * Text is an Object that wraps an IText and exposes it to the engine's
   * object pipeline: it can be updated every frame, drawn through the
   * Batcher, and saved to / loaded from an ObjectState.
   *
   * The text content and the font size can be changed at runtime through
   * ChangeText() and ChangeSizeFont().
   */
  class Text : public Object
  {
    public:
      /**
       * @brief Creates a text object.
       *
       * @param[in] postion Position of the text in the scene.
       * @param[in] fontSize Font size used to render the text.
       * @param[in] text Initial text content.
       */
      Text(const Vector2& postion, float fontSize, const std::string& text);
      /// @brief Default destructor.
      ~Text() override {};
      /**
       * @brief Submits the text to the given Batcher for rendering.
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
      void ChangeText(const std::string& newText)
      {
        this->m_text.SetText(newText);
      }
      /**
       * @brief Changes the font size used to render the text.
       *
       * @param[in] newFontSize New font size.
       */
      void ChangeSizeFont(float newFontSize)
      {
        this->m_text.SetFontSize(newFontSize);
      }
      /**
       * @brief Serializes the current state of the text object.
       *
       * @return An ObjectState describing this object, suitable for
       *         restoring it later with Load().
       */
      ObjectState Save() override;
      /**
       * @brief Restores the text object from a previously saved state.
       *
       * @param[in] state State produced by Save().
       */
      void Load(const ObjectState& state) override;
    private:
      IText m_text; /// @brief Underlying text data (content, font size, etc.) that this object renders.
  };
}
