// | -------------------------------
#include "TextBox.hpp"
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RAPIBatch.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  TextBox::TextBox(const std::string& name, const std::string& text, const Vector2& dimention)
    : Object(name),m_text{text},m_textColor{Color::Black}, m_bgColor{Color::White}, m_outlineColor{Color::Black}
  {
    m_box = &AddBoundingBox(dimention);


    // Size of the font
    // auto sise = TextAPI::Get().GetMeasureTextEx(this->m_text.GetFont(), this->m_text.GetText(), this->m_text.GetFontSize());
    // m_textPosition;
  }

  void TextBox::Update(float dt)
  {
    
  }

  void TextBox::Draw(Batcher& b) const
  {
    if(m_sprite)
    {
      b.DrawAtlasSprite(this->GetPosition(),m_box->GetSize(), m_sprite->atlasLayer, m_textureRect.uvMin, m_textureRect.uvMax);
    }
    else
    {
      Rectangle rect = {transform.m_position.x, transform.m_position.y,m_box->GetSize().x,m_box->GetSize().y};
      Drawer::DrawRectangle(rect,this->m_bgColor.GetColor());
      Drawer::DrawRectangleOutline(rect,this->m_outlineColor.GetColor());
    }
    TextAPI::Get().DrawTextEx(*this->m_text.GetFont(), this->m_textPosition, this->m_text.GetText(), this->m_text.GetFontSize(), this->m_textColor.GetColor());
  }

  void TextBox::SetPosition(const Vector2& position)
  {
    this->GetTransform().m_position = position;
  }

  void TextBox::SetDimention(const Vector2& dimention)
  {
    this->m_box->ChangeSize(dimention);
  }

  void TextBox::SetText(const std::string& text)
  {
    this->m_text.SetText(text);
  }

  void TextBox::SetFontSize(float size)
  {
    this->m_text.SetFontSize(size);
  }

  void TextBox::SetTextColor(const Color& color)
  {
    this->m_textColor.ChangeColor(color);
  }

  void TextBox::SetFont(Font& font)
  {
    this->m_text.SetFont(&font);
  }

  std::string TextBox::GetTextStorage(void) const
  {
    return this->m_textStorage;
  }

  void TextBox::SetBackgroundColor(const Color& color)
  {
    this->m_bgColor.ChangeColor(color);
  }

  void TextBox::SetOutlineColor(const Color& color)
  {
    this->m_outlineColor.ChangeColor(color);
  }

  void TextBox::SetDelimiter(const char& delimiter)
  {
    this->m_crDelimiter = delimiter;
  }
}
