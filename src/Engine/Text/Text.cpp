/// | -------------------------------
#include "Text.hpp"
/// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | -------------------------------
#include <string>
/// | -------------------------------

namespace ENG
{
  Text::Text(const Vector2& position,float fontSize, const std::string& text)
    : Object(text), m_text{text}, m_fontSize{fontSize}
  {
    this->SetPosition(position);
  }

  void Text::Draw(Batcher& b) const
  {
    const auto font = Services::Fonts().GetFont("PottaOne"); 
    const auto color =this->GetComponent<ENG::IColor>()->GetColor();
    TextAPI::Get().DrawTextEx(*font,this->GetPosition(),this->m_text, m_fontSize, color);
  }

  void Text::Update(float dt)
  {
    (void)dt;
  }
}
