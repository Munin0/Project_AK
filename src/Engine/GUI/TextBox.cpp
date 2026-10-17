// | -------------------------------
#include "TextBox.hpp"
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Inputs/Mouse.hpp"
#include "Engine/Inputs/PollEvent.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Physics/Collision.hpp"
#include "Engine/Render/Batching/RAPIBatch.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "SDL3/SDL_mouse.h"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  TextBox::TextBox(const std::string& name, const std::string& text, const Vector2& dimention)
    : Object(name),m_text{text},m_textColor{Color::Black}, m_bgColor{Color::White}, m_outlineColor{Color::Black}, m_isPressed{false}
  {
    m_box = &AddBoundingBox(dimention);
  }

  void TextBox::Update(float dt)
  {
    if(!m_ConfigComplete)
    {
      auto size = TextAPI::Get().GetMeasureTextEx(m_text.GetFont(),m_text.GetText());
      m_textPosition = {
        GetTransform().m_position.x + 10.0f,
        GetTransform().m_position.y + m_box->GetSize().y / 2.0f - size.y / 2.0f
      };
      m_ConfigComplete = true; 
    }

    auto& pEvent = PollEvent::Get();
    if(CollisionPointRect(GetMousePosition(), m_box->GetRectangleBB()) && pEvent.IsMouseButtonPress(SDL_BUTTON_LEFT))
    {
      m_isPressed = true;
    }
    if(CollisionPointRect(GetMousePosition(), m_box->GetRectangleBB()) == false && pEvent.IsMouseButtonPress(SDL_BUTTON_LEFT))
    {
      m_isPressed = false;
    }

    if(m_isPressed)
    {
      
    }
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
    if(m_isPressed)
    {
      TextAPI::Get().DrawTextEx(*this->m_text.GetFont(), this->m_textPosition, std::to_string(m_crDelimiter), this->m_text.GetFontSize(), this->m_textColor.GetColor());
    }
    else
    {
      TextAPI::Get().DrawTextEx(*this->m_text.GetFont(), this->m_textPosition, this->m_text.GetText(), this->m_text.GetFontSize(), this->m_textColor.GetColor());
    }


  }

  void TextBox::SetPosition(const Vector2& position)
  {
    this->GetTransform().m_position = position;
    m_ConfigComplete = false;
  }

  void TextBox::SetDimention(const Vector2& dimention)
  {
    this->m_box->ChangeSize(dimention);
    m_ConfigComplete = false;
  }

  void TextBox::SetText(const std::string& text)
  {
    this->m_text.SetText(text);
    m_ConfigComplete = false;
  }

  void TextBox::SetFontSize(float size)
  {
    this->m_text.SetFontSize(size);
    m_ConfigComplete = false;
  }

  void TextBox::SetTextColor(const Color& color)
  {
    this->m_textColor.ChangeColor(color);
    m_ConfigComplete = false;
  }

  void TextBox::SetFont(Font& font)
  {
    this->m_text.SetFont(&font);
    m_ConfigComplete = false;
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
