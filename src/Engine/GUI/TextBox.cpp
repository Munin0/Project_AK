// | -------------------------------
#include "TextBox.hpp"
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Inputs/PollEvent.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Physics/Collision.hpp"
#include "Engine/Render/Batching/Drawer.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Color/Color.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Text/Utf8.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_scancode.h"
// | -------------------------------
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
// | -------------------------------

namespace ENG
{
  namespace
  {
    /// Left/right inner margin of the text, in pixels.
    constexpr float PADDING = 10.0f;
    /// Caret blink period, in seconds (visible the first half).
    constexpr float CARET_PERIOD = 1.0f;
  }

  TextBox::TextBox(const std::string& name, const std::string& placeholder, const Vector2& dimention)
    : Object(name), m_placeholder{placeholder}, m_text{""}, m_textColor{Color::Black},
      m_placeholderColor{Color::Gray}, m_selectionColor{Color(0.6f, 0.8f, 1.0f, 1.0f)},
      m_bgColor{Color::White}, m_outlineColor{Color::Black}
  {
    m_box = &AddComponent<IBoundingBox>(dimention);
  }

  TextBox::~TextBox()
  {
    // Removed from the pool while typing (scene change, Remove): give the keyboard back.
    PollEvent::Get().StopTextInput(this);
  }

  void TextBox::Update(float dt)
  {
    if(!m_ConfigComplete && m_text.GetFont())   // without a font there is nothing to measure yet
    {
      // Same vertical centering as Label (the text is drawn from its baseline).
      // Measured with a fixed sample so the line does not jump while typing.
      auto size = TextAPI::Get().GetMeasureTextEx(m_text.GetFont(), "Ag", m_text.GetFontSize());
      m_textPosition = {
        GetTransform().m_position.x + PADDING,
        GetTransform().m_position.y + m_box->GetSize().y / 2.0f + size.y / 4.0f
      };
      m_ConfigComplete = true;
    }

    if(!m_isPressed)
      return;

    // Hidden or the text input was taken by another box: drop the focus.
    if(!IsVisible() || !PollEvent::Get().IsTextInputOwner(this))
    {
      Unfocus();
      return;
    }

    m_caretTimer += dt;
    auto& in = PollEvent::Get();

    if(!in.GetTextInput().empty())
    {
      Insert(in.GetTextInput());
      m_caretTimer = 0.0f;   // keep the caret visible while typing
    }

    for(const TypedKey& typed : in.GetTypedKeys())
    {
      switch(typed.m_key)
      {
        case SDL_SCANCODE_A:           // Ctrl+A: select all (a plain "a" comes as text)
          if(!typed.Ctrl())
            continue;
          SelectAll();
          break;
        case SDL_SCANCODE_BACKSPACE:   // the selection, or the character before the caret
          if(HasSelection())
            DeleteSelection();
          else if(m_caret > 0)
          {
            const size_t start = Utf8::Prev(m_textStorage, m_caret);
            m_textStorage.erase(start, m_caret - start);
            MoveCaret(start);
          }
          break;
        case SDL_SCANCODE_DELETE:      // the selection, or the character after the caret
          if(HasSelection())
            DeleteSelection();
          else if(m_caret < m_textStorage.size())
            m_textStorage.erase(m_caret, Utf8::NextOffset(m_textStorage, m_caret) - m_caret);
          break;
        case SDL_SCANCODE_LEFT:        // with a selection: go to its start
          MoveCaret(HasSelection() ? std::min(m_anchor, m_caret) : Utf8::Prev(m_textStorage, m_caret));
          break;
        case SDL_SCANCODE_RIGHT:       // with a selection: go to its end
          MoveCaret(HasSelection() ? std::max(m_anchor, m_caret) : Utf8::NextOffset(m_textStorage, m_caret));
          break;
        case SDL_SCANCODE_HOME:
          MoveCaret(0);
          break;
        case SDL_SCANCODE_END:
          MoveCaret(m_textStorage.size());
          break;
        case SDL_SCANCODE_RETURN:
        case SDL_SCANCODE_KP_ENTER:
        {
          // copy: the callback may change the box
          SubmitCallback callback = m_onSubmit;
          Unfocus();
          if(callback)
            callback(m_textStorage);
          return;
        }
        case SDL_SCANCODE_ESCAPE:
          Unfocus();
          return;
        default:
          continue;   // not an editing key: keep the caret blink as it is
      }
      m_caretTimer = 0.0f;   // show the caret right after it moves or the text changes
    }

    UpdateScroll();
  }

  void TextBox::OnPointer(const Vector2& mouse)
  {
    auto& pEvent = PollEvent::Get();
    m_hover = CollisionPointRect(mouse, m_box->GetRectangleBB());

    // clicked: take the focus
    if(m_hover && pEvent.ConsumeMouseButtonPress(SDL_BUTTON_LEFT))
      Focus();
    // clicked elsewhere (even if taken)
    else if(m_isPressed && pEvent.IsMouseButtonPressRaw(SDL_BUTTON_LEFT))
      Unfocus();
  }

  void TextBox::Draw(Batcher& b) const
  {
    if(m_sprite)
    {
      b.DrawAtlasSprite(this->GetPosition(),m_box->GetSize(), m_sprite->m_atlasLayer, m_textureRect.m_uvMin, m_textureRect.m_uvMax);
    }
    else
    {
      Rectangle rect = {m_transform.m_position.x, m_transform.m_position.y,m_box->GetSize().x,m_box->GetSize().y};
      Drawer::DrawRectangle(rect,this->m_bgColor.GetColor());
      Drawer::DrawRectangleOutline(rect,this->m_outlineColor.GetColor());
    }

    const Font* font = m_text.GetFont();
    if(!font)
      return;
    auto& api = TextAPI::Get();
    const float size = m_text.GetFontSize();

    // Empty and unfocused: the hint.
    if(m_textStorage.empty() && !m_isPressed)
    {
      api.DrawTextEx(*font, m_textPosition, m_placeholder, size, m_placeholderColor.GetColor());
      return;
    }

    // Visible part: from m_scroll (kept by UpdateScroll) while it fits in the box.
    const size_t scroll = std::min(m_scroll, m_textStorage.size());
    const float maxWidth = VisibleWidth();
    std::string shown = m_textStorage.substr(scroll);
    while(!shown.empty() && api.GetMeasureTextEx(font, shown, size).x > maxWidth)
      shown.erase(Utf8::Prev(shown, shown.size()));

    // Selection highlight, behind the text and clipped to the visible part.
    if(m_isPressed && HasSelection())
    {
      auto xOf = [&](size_t offset)   // x of a byte offset of m_textStorage, inside the view
      {
        offset = std::clamp(offset, scroll, scroll + shown.size());
        return offset == scroll ? 0.0f
             : api.GetMeasureTextEx(font, m_textStorage.substr(scroll, offset - scroll), size).x;
      };
      const float x0 = xOf(std::min(m_anchor, m_caret));
      const float x1 = xOf(std::max(m_anchor, m_caret));
      const float h  = api.GetMeasureTextEx(font, "Ag", size).y;
      const float cy = m_transform.m_position.y + m_box->GetSize().y / 2.0f;
      if(x1 > x0)
        Drawer::DrawRectangle({m_textPosition.x + x0, cy - h / 2.0f, x1 - x0, h}, m_selectionColor.GetColor());
    }

    api.DrawTextEx(*font, m_textPosition, shown, size, m_textColor.GetColor());

    // Blinking caret at its position (the width of the visible text before it).
    if(m_isPressed && std::fmod(m_caretTimer, CARET_PERIOD) < CARET_PERIOD * 0.5f)
    {
      const std::string before = m_caret > scroll ? m_textStorage.substr(scroll, m_caret - scroll) : "";
      const float width = before.empty() ? 0.0f : api.GetMeasureTextEx(font, before, size).x;
      const std::string caret(1, m_crDelimiter);
      api.DrawTextEx(*font, {m_textPosition.x + width + 1.0f, m_textPosition.y}, caret, size, m_textColor.GetColor());
    }
  }

  void TextBox::Focus(void)
  {
    m_isPressed  = true;
    m_caretTimer = 0.0f;
    m_caret      = m_textStorage.size();   // start typing at the end
    m_anchor     = m_caret;
    UpdateScroll();
    PollEvent::Get().StartTextInput(this);
  }

  void TextBox::Unfocus(void)
  {
    m_isPressed = false;
    PollEvent::Get().StopTextInput(this);
  }

  void TextBox::SelectAll(void)
  {
    m_anchor = 0;
    m_caret  = m_textStorage.size();
  }

  void TextBox::DeleteSelection(void)
  {
    const size_t start = std::min(m_anchor, m_caret);
    const size_t end   = std::max(m_anchor, m_caret);
    m_textStorage.erase(start, end - start);
    MoveCaret(start);
  }

  void TextBox::MoveCaret(size_t pos)
  {
    m_caret  = std::min(pos, m_textStorage.size());
    m_anchor = m_caret;
  }

  void TextBox::Insert(const std::string& text)
  {
    // Typing over a selection replaces it (only if there is something to type).
    if(HasSelection())
    {
      bool hasText = false;
      for(size_t i = 0; i < text.size() && !hasText;)
      {
        const uint32_t cp = Utf8::Next(text, i);
        hasText = cp >= 0x20 && cp != 0x7F;
      }
      if(!hasText)
        return;
      DeleteSelection();
    }

    size_t length = Utf8::Length(m_textStorage);
    for(size_t i = 0; i < text.size();)
    {
      if(m_maxLength != 0 && length >= m_maxLength)
        break;
      const size_t start = i;
      const uint32_t cp  = Utf8::Next(text, i);
      if(cp < 0x20 || cp == 0x7F)   // control characters are not text
        continue;
      m_textStorage.insert(m_caret, text, start, i - start);
      m_caret += i - start;
      length++;
    }
    m_anchor = m_caret;
  }

  float TextBox::VisibleWidth(void) const
  {
    const std::string caret(1, m_crDelimiter);
    const float caretWidth = TextAPI::Get().GetMeasureTextEx(m_text.GetFont(), caret, m_text.GetFontSize()).x;
    return m_box->GetSize().x - 2.0f * PADDING - caretWidth;
  }

  void TextBox::UpdateScroll(void)
  {
    m_caret  = std::min(m_caret, m_textStorage.size());
    m_scroll = std::min(m_scroll, m_caret);   // caret went left of the view: start there
    const Font* font = m_text.GetFont();
    if(!font)
      return;

    auto& api        = TextAPI::Get();
    const float size = m_text.GetFontSize();
    const float max  = VisibleWidth();
    auto width = [&](size_t from, size_t to)
    {
      return from >= to ? 0.0f : api.GetMeasureTextEx(font, m_textStorage.substr(from, to - from), size).x;
    };

    // Caret right of the view: scroll right until it fits.
    while(m_scroll < m_caret && width(m_scroll, m_caret) > max)
      m_scroll = Utf8::NextOffset(m_textStorage, m_scroll);

    // Space left at the end (text deleted): scroll back left while the rest still fits.
    while(m_scroll > 0 && width(Utf8::Prev(m_textStorage, m_scroll), m_textStorage.size()) <= max)
      m_scroll = Utf8::Prev(m_textStorage, m_scroll);
  }

  void TextBox::SetPosition(const Vector2& position)
  {
    Object::SetPosition(position);   // also moves the bounding box (the click area)
    m_ConfigComplete = false;
  }

  void TextBox::SetDimension(const Vector2& dimension)
  {
    this->m_box->ChangeSize(dimension);
    m_ConfigComplete = false;
  }

  void TextBox::SetText(const std::string& text)
  {
    m_textStorage.clear();
    m_caret  = 0;
    m_anchor = 0;
    m_scroll = 0;
    Insert(text);   // leaves the caret at the end
  }

  void TextBox::SetPlaceholder(const std::string& placeholder)
  {
    m_placeholder = placeholder;
  }

  void TextBox::SetMaxLength(size_t maxLength)
  {
    m_maxLength = maxLength;
    if(m_maxLength != 0 && Utf8::Length(m_textStorage) > m_maxLength)
      m_textStorage.erase(Utf8::ByteOffset(m_textStorage, m_maxLength));
    m_caret  = std::min(m_caret, m_textStorage.size());
    m_anchor = std::min(m_anchor, m_textStorage.size());
    m_scroll = std::min(m_scroll, m_caret);
  }

  void TextBox::SetOnSubmit(SubmitCallback callback)
  {
    m_onSubmit = std::move(callback);
  }

  void TextBox::SetFontSize(float size)
  {
    this->m_text.SetFontSize(size);
    m_ConfigComplete = false;
  }

  void TextBox::SetTextColor(const Color& color)
  {
    this->m_textColor.ChangeColor(color);
  }

  void TextBox::SetSelectionColor(const Color& color)
  {
    this->m_selectionColor.ChangeColor(color);
  }

  void TextBox::SetPlaceholderColor(const Color& color)
  {
    this->m_placeholderColor.ChangeColor(color);
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

  void TextBox::SetInteraction(bool _)
  {
    if(_)
      Focus();
    else if(m_isPressed)
      Unfocus();
  }
}
