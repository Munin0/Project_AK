// | -------------------------------
#include "TextReveal.hpp"
// | -------------------------------
#include "Engine/Text/Utf8.hpp"
// | -------------------------------
#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
// | -------------------------------

namespace ENG
{
  void TextReveal::Start(std::string text, float charsPerSecond)
  {
    m_text         = std::move(text);
    m_totalChars   = Utf8::Length(m_text);
    m_speed        = charsPerSecond;
    m_progress     = 0.0f;
    m_visibleChars = 0;
    m_visibleBytes = 0;
    if (m_speed <= 0.0f)
      Finish();
  }

  void TextReveal::Show(std::string text)
  {
    Start(std::move(text), 0.0f);
  }

  void TextReveal::Update(float dt)
  {
    if (IsDone())
      return;

    m_progress += m_speed * dt;
    const size_t chars = std::min(static_cast<size_t>(m_progress), m_totalChars);
    if (chars != m_visibleChars)
    {
      m_visibleChars = chars;
      m_visibleBytes = Utf8::ByteOffset(m_text, m_visibleChars);
    }
  }

  void TextReveal::Finish()
  {
    m_visibleChars = m_totalChars;
    m_visibleBytes = m_text.size();
    m_progress     = static_cast<float>(m_totalChars);
  }

  std::string_view TextReveal::GetVisible() const
  {
    return std::string_view(m_text).substr(0, m_visibleBytes);
  }
}
