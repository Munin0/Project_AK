// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Geometry/RGeometry.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Rects.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <any>
#include <functional>
#include <string>
// | -------------------------------

namespace ENG
{
  class Button : public Object
  { public:
      using ButtonCallback = std::function<void(std::any)>;

      Button(const std::string& _name, const Vector2& dimention);
      Button(const std::string& _name, const Vector2& dimention, std::function<int(std::any)> _func);
      ~Button() override {};

      void Draw(Batcher& b) const override;
      void Update(float dt) override;
      void Action(std::any _);

      void SetTexture(const std::string& atlas, const std::string& textName);
      void SetNextScene(const SceneID& id);
      void SetFunction(ButtonCallback func);
      void SetData(std::any _);
      void SetText(const std::string& text);
      void SetTextSize(float size);
      void SetTextColor(const Color& color);
      void SetFont(Font& font);

      std::any& GetData(void);
      const std::any& GetData(void) const;

      Rect GetRect(void);
    private:
      ButtonCallback m_callback = nullptr; 
      std::any m_data;
      std::string m_text;
      Font* m_font;
      float m_fontSize;
      Color m_fontColor;

      const IBoundingBox* m_box = nullptr;
      const AtlasData* m_sprite = nullptr;
      UVRect m_textureRect{};

      bool m_hasTexture = false;
      bool m_pressed = false;
  };
}
