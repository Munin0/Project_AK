// | -------------------------------
#include "Button.hpp"
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RAPIBatch.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Geometry/RGeometry.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Rects.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <any>
#include <functional>
#include <string>
#include <utility>
// | -------------------------------

namespace ENG
{
  Button::Button(const std::string& _name, const Vector2& dimention)
    : Object{_name}, m_callback{nullptr}
  {
    m_box = &AddBoundingBox(dimention);
    stats.hp = 1;
    stats.hp_max = 0;
    stats.def = 0;
    stats.agi = 0;
    stats.str = 0;
  }

  Button::Button(const std::string& _name, const Vector2& dimention, std::function<int(std::any)> _func)
    : Object{_name}, m_callback{_func}
  { 
    m_box = &AddBoundingBox(dimention);
  }

  void Button::Draw(Batcher& b) const
  {
    if(m_sprite != nullptr)
    {
      b.DrawAtlasSprite(transform.position, m_box->GetSize(), m_sprite->atlasLayer, m_textureRect.uvMin, m_textureRect.uvMax,{1,1,1,1},0.0f);
    }
    else
    {
      Rect rect = {transform.position.x, transform.position.y,m_box->GetSize().x,m_box->GetSize().y};
      Drawer::DrawRectangle(rect,Color::White);
      Drawer::DrawRectangleOutline(rect,Color::Black);
    }

    if(!m_text.empty() && m_font != nullptr)
    {
      auto size = this->m_box->GetSize();
      auto textSize = TextAPI::Get().GetMeasureTextEx(this->m_font,this->m_text, this->m_fontSize);
      Vector2 fontPosition = {
        (this->transform.position.x + size.x / 2.0f) - (textSize.x / 2.0f),
        (this->transform.position.y + size.y / 2.0f) + (textSize.y / 4.0f)
      };
      TextAPI::Get().DrawTextEx(*m_font, fontPosition, this->m_text, this->m_fontSize, this->m_fontColor);
    }
  }
  
  void Button::Update(float dt)
  {
    m_box = GetSolidBox();
  }

  void Button::Action(std::any _)
  {
    if(m_callback == nullptr)
      return;
    this->m_callback(_);
  }


  void Button::SetTexture(const std::string& atlas, const std::string& textName)
  {
    m_sprite = Services::Assets().GetAtlas(atlas);
    if(m_sprite != nullptr)
    {
      m_textureRect = GetTileUV(*m_sprite,textName);
    }
  }

  void Button::SetNextScene(const SceneID& id)
  {
    this->m_data = id;
  }

  void Button::SetFunction(ButtonCallback func)
  {
    if(func == nullptr)
    {
      LOG_ERROR(" | << Function not asigned to " + this->GetName());
      this->m_callback = nullptr;
    }
    this->m_callback = func;
  }

  void Button::SetFont(Font& font)
  {
    this->m_font = &font;
  }

  void Button::SetData(std::any _)
  {
    this->m_data = std::move(_);
  }
  
  void Button::SetText(const std::string& text)
  {
    this->m_text = text;
  }

  void Button::SetTextSize(float size)
  {
    this->m_fontSize = size;
  }

  void Button::SetTextColor(const Color& color)
  {
    this->m_fontColor = color;
  }

  std::any& Button::GetData(void)
  {
    return this->m_data;
  }

  const std::any& Button::GetData(void) const
  {
    return this->m_data;
  }

  Rect Button::GetRect(void)
  {
    const Vector2 size = GetSolidBox()->GetSize();
    const Vector2 pos = GetTransform().position;

    return {pos.x,pos.y,size.x,size.y};
  }
}
