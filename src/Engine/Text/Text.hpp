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
  class Text : public Object
  {
    public:
      Text(const Vector2& postion, float fontSize, const std::string& text);
      ~Text() override {};

      void Draw(Batcher& b) const override;
      void Update(float dt) override;

      void ChangeText(const std::string& newText)
      {
        this->m_text.SetText(newText);
      }
      void ChangeSizeFont(float newFontSize)
      {
        this->m_text.SetFontSize(newFontSize);
      }
      
      ObjectState Save() override;
      void Load(const ObjectState& state) override;

    private:
      IText m_text;
  };
} // namespace ENG
