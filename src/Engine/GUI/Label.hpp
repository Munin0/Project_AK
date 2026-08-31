/// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  enum class AlignText
  {
    TEXT_ALIGN_UPL    = 0,
    TEXT_ALIGN_UPR    = 1,
    TEXT_ALIGN_DOWNL  = 2,
    TEXT_ALIGN_DOWNR  = 3,
    TEXT_ALIGN_CENTER = 4
  };
    
  class Label : public Object
  {
    public:
      Label(const std::string& name, const Vector2& dimention);
      ~Label() override {};

      void Draw(Batcher& b) const override;
      void Update(float dt) override;
      
      void SetText(const std::string& text);
      void SetFont(Font& font);
      void SetColorText(const Color& color);
      void SetFontSize(float size);
      void SetOffset(const Vector2& offset);
      void SetBackgoundColor(const Color& bgColor);
      void SetTexture(const std::string& key, const std::string& textName);

      void SetTextAlign(AlignText position);

    private:
      std::string m_text;
      Font* m_font;
      float m_fontSize;
      Color m_colorText;
      Vector2 m_fontPosition;

      const IBoundingBox* m_box;
      Color m_bgColor;

      const AtlasData* m_sprite = nullptr;
      UVRect m_textureRect;
      Vector2 m_offset;
  };
}
