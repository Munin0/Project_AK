// | -------------------------------
#include "Label.hpp"
// | -------------------------------
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RAPIBatch.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  Label::Label(const std::string& name, const Vector2& dimention)
    : Object(name), m_text{""}, m_colorText{Color::Black}, m_bgColor{Color::White}, m_outlineColor{Color::Black}
  {
    this->m_box = &AddBoundingBox(dimention);
  }

  void Label::Draw(Batcher& b) const
  {
    /// Sprite option
    if(m_sprite != nullptr)
    {
      b.DrawAtlasSprite(transform.m_position, m_box->GetSize(), m_sprite->atlasLayer, m_textureRect.uvMin, m_textureRect.uvMax, m_colorText.GetColor());
    }
    else
    {
      Rectangle rect = {transform.m_position.x, transform.m_position.y,m_box->GetSize().x,m_box->GetSize().y};
      Drawer::DrawRectangle(rect,this->m_bgColor.GetColor());
      Drawer::DrawRectangleOutline(rect,this->m_outlineColor.GetColor());
    }
    TextAPI::Get().DrawTextEx(*this->m_text.GetFont(), this->m_fontPosition, this->m_text.GetText(), this->m_text.GetFontSize(), this->m_colorText.GetColor());
  }

  void Label::Update(float dt)
  {
    
  }

  void Label::SetText(const std::string& text)
  {
    this->m_text = text;
  }

  void Label::SetFont(Font& font)
  {
    this->m_text.SetFont(&font);
  }

  void Label::SetColorText(const Color& color)
  {
    this->m_colorText.ChangeColor(color);
  }

  void Label::SetFontSize(float size)
  {
    this->m_text.SetFontSize(size);
  }

  void Label::SetBackgoundColor(const Color& bgColor)
  {
    this->m_bgColor.ChangeColor(bgColor);
  }

  void Label::SetTexture(const std::string& atlas, const std::string& textName)
  {
    m_sprite = Services::Assets().GetAtlas(atlas);
    if(m_sprite != nullptr)
    {
      m_textureRect = GetTileUV(*m_sprite,textName);
    }
  }

  void Label::SetOffset(const Vector2& offset)
  {
    this->m_offset = offset;
  }
  
  void Label::SetTextAlign(AlignText position)
  {
    switch(position) 
    {
      case AlignText::TEXT_ALIGN_UPL:
      {
        this->m_fontPosition = {
          this->transform.m_position.x + this->m_offset.x, 
          this->transform.m_position.y + this->m_text.GetFontSize() / 2.0f + this->m_offset.y
        };
        break;
      }
      case AlignText::TEXT_ALIGN_UPR:
      {
        auto size = this->m_box->GetSize();
        auto textSize = TextAPI::Get().GetMeasureTextEx(this->m_text.GetFont(),this->m_text.GetText(), this->m_text.GetFontSize());
        this->m_fontPosition = {
          (this->transform.m_position.x + size.x) - textSize.x - this->m_offset.x,
          (this->transform.m_position.y + this->m_offset.y) + textSize.y / 2.0f
        };
        break;
      }
      case AlignText::TEXT_ALIGN_DOWNL:
      {
        auto size = this->m_box->GetSize();
        auto textSize = TextAPI::Get().GetMeasureTextEx(this->m_text.GetFont(),this->m_text.GetText(), this->m_text.GetFontSize());
        this->m_fontPosition = {
          this->transform.m_position.x + this->m_offset.x, 
          this->transform.m_position.y + size.y - this->m_offset.y
        };
        break;
      }
      case AlignText::TEXT_ALIGN_DOWNR:
      {
        auto size = this->m_box->GetSize();
        auto textSize = TextAPI::Get().GetMeasureTextEx(this->m_text.GetFont(),this->m_text.GetText(), this->m_text.GetFontSize());
        this->m_fontPosition = {
          (this->transform.m_position.x + size.x) - textSize.x - this->m_offset.x ,
          (this->transform.m_position.y + size.y) - this->m_offset.y
        };
        break;
      }
      case AlignText::TEXT_ALIGN_CENTER:
      {
        auto size = this->m_box->GetSize();
        auto textSize = TextAPI::Get().GetMeasureTextEx(this->m_text.GetFont(),this->m_text.GetText(), this->m_text.GetFontSize());
        this->m_fontPosition = {
          (this->transform.m_position.x + size.x / 2.0f) - (textSize.x / 2.0f),
          (this->transform.m_position.y + size.y / 2.0f) + (textSize.y / 4.0f)
         };
        break;
      }
      default:
      {
        LOG_ERROR(" Position not valid");
        break;
      }
    }
  }

}
