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
#include "Engine/Utils/Rects.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  Label::Label(const std::string& name, const Vector2& dimention)
    : Object(name)
  {
    this->m_box = &AddBoundingBox(dimention);
  }

  void Label::Draw(Batcher& b) const
  {
    /// Sprite option
    if(m_sprite != nullptr)
    {
      b.DrawAtlasSprite(transform.position, m_box->GetSize(), m_sprite->atlasLayer, m_textureRect.uvMin, m_textureRect.uvMax);
    }
    else
    {
      Rect rect = {transform.position.x, transform.position.y,m_box->GetSize().x,m_box->GetSize().y};
      Drawer::DrawRectangle(rect,this->m_bgColor);
    }
    TextAPI::Get().DrawTextEx(*this->m_font, this->m_fontPosition, this->m_text, this->m_fontSize, this->m_colorText);
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
    m_font = &font;
  }

  void Label::SetColorText(const Color& color)
  {
    this->m_colorText = color;
  }

  void Label::SetFontSize(float size)
  {
    this->m_fontSize = size;
  }

  void Label::SetBackgoundColor(const Color& bgColor)
  {
    this->m_bgColor = bgColor;
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
          this->transform.position.x + this->m_offset.x, 
          this->transform.position.y + this->m_fontSize / 2.0f + this->m_offset.y
        };
        break;
      }
      case AlignText::TEXT_ALIGN_UPR:
      {
        auto size = this->m_box->GetSize();
        auto textSize = TextAPI::Get().GetMeasureTextEx(this->m_font,this->m_text, this->m_fontSize);
        this->m_fontPosition = {
          (this->transform.position.x + size.x) - textSize.x - this->m_offset.x,
          (this->transform.position.y + this->m_offset.y) + textSize.y / 2.0f
        };
        break;
      }
      case AlignText::TEXT_ALIGN_DOWNL:
      {
        auto size = this->m_box->GetSize();
        auto textSize = TextAPI::Get().GetMeasureTextEx(this->m_font,this->m_text, this->m_fontSize);
        this->m_fontPosition = {
          this->transform.position.x + this->m_offset.x, 
          this->transform.position.y + size.y - this->m_offset.y
        };
        break;
      }
      case AlignText::TEXT_ALIGN_DOWNR:
      {
        auto size = this->m_box->GetSize();
        auto textSize = TextAPI::Get().GetMeasureTextEx(this->m_font,this->m_text, this->m_fontSize);
        this->m_fontPosition = {
          (this->transform.position.x + size.x) - textSize.x - this->m_offset.x ,
          (this->transform.position.y + size.y) - this->m_offset.y
        };
        break;
      }
      case AlignText::TEXT_ALIGN_CENTER:
      {
        auto size = this->m_box->GetSize();
        auto textSize = TextAPI::Get().GetMeasureTextEx(this->m_font,this->m_text, this->m_fontSize);
        this->m_fontPosition = {
          (this->transform.position.x + size.x / 2.0f) - (textSize.x / 2.0f),
          (this->transform.position.y + size.y / 2.0f) + (textSize.y / 4.0f)
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
