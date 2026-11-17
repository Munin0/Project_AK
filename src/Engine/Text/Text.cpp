/// | -------------------------------
#include "Text.hpp"
/// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Color/Color.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | -------------------------------
#include <string>
/// | -------------------------------

namespace ENG
{
  Text::Text(const Vector2& position, float fontSize, const std::string& text)
    : Object(text)
  {
    auto& t = AddComponent<IText>(text);
    t.SetFontSize(fontSize);
    this->SetPosition(position);
  }

  void Text::Draw(Batcher& b) const
  {
    (void)b;
    const IText* t = TextData();
    if(!t || !t->GetFont())
      return;

    Color color = Color::White;
    if(const auto* c = GetComponent<IColor>())
      color = c->GetColor();

    TextAPI::Get().DrawTextEx(*t->GetFont(), GetPosition(), t->GetText(), t->GetFontSize(), color);
  }

  void Text::Update(float dt)
  {
    (void)dt;
  }

  void Text::ChangeText(const std::string& newText)
  {
    if(auto* t = TextData())
      t->SetText(newText);
  }

  void Text::ChangeSizeFont(float newFontSize)
  {
    if(auto* t = TextData())
      t->SetFontSize(newFontSize);
  }

  void Text::SetFont(Font* font)
  {
    if(auto* t = TextData())
      t->SetFont(font);
  }

  std::string Text::GetText() const
  {
    const auto* t = TextData();
    return t ? t->GetText() : std::string{};
  }

  float Text::GetFontSize() const
  {
    const auto* t = TextData();
    return t ? t->GetFontSize() : 0.0f;
  }

  IText* Text::TextData() const
  {
    return GetComponent<IText>();
  }
}
