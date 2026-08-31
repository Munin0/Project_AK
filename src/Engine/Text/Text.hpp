/// | -------------------------------
#pragma once
/// | -------------------------------
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | -------------------------------
/// | -------------------------------
#include <string>
/// | -------------------------------

namespace ENG
{
  class Text : public Object
  {
    public:
      Text(const Vector2& postion, float fontSize, const std::string& text);
      ~Text() override {};

      void Draw(Batcher& b) const override;
      void Update(float dt) override;

      void ChangeText(const std::string& newText)
      {
        this->m_text = newText;
      }
      void ChangeSizeFont(float newFontSize);
    private:
      std::string m_text;
      float       m_fontSize;
  };
} // namespace ENG
