// | -------------------------------
#pragma once
// | -------------------------------
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
// | -------------------------------

/**
 * @brief Minimal UTF-8 helpers for the text code.
 *
 * Texts are stored as UTF-8 std::string ("¿Cómo estás?"), while the glyphs of
 * a Font are indexed by Unicode codepoint. These helpers walk a string one
 * codepoint (not one byte) at a time.
 */
namespace ENG::Utf8
{
  /// @brief Codepoint returned for malformed sequences.
  inline constexpr uint32_t REPLACEMENT = 0xFFFD;

  /**
   * @brief Decodes the codepoint that starts at byte @p i and advances @p i past it.
   *
   * Malformed or truncated sequences return REPLACEMENT and advance one byte,
   * so the loop always makes progress.
   *
   * @code
   * for (size_t i = 0; i < text.size();)
   *   uint32_t cp = ENG::Utf8::Next(text, i);
   * @endcode
   */
  inline uint32_t Next(std::string_view s, size_t& i)
  {
    const auto byte = [&](size_t k) { return static_cast<uint8_t>(s[k]); };
    const uint8_t b0 = byte(i);

    size_t   len = 0;
    uint32_t cp  = 0;
    if (b0 < 0x80)                { i += 1; return b0; }
    else if ((b0 & 0xE0) == 0xC0) { len = 2; cp = b0 & 0x1F; }
    else if ((b0 & 0xF0) == 0xE0) { len = 3; cp = b0 & 0x0F; }
    else if ((b0 & 0xF8) == 0xF0) { len = 4; cp = b0 & 0x07; }
    else                          { i += 1; return REPLACEMENT; }

    if (i + len > s.size())
    {
      i += 1;
      return REPLACEMENT;
    }
    for (size_t k = 1; k < len; k++)
    {
      const uint8_t b = byte(i + k);
      if ((b & 0xC0) != 0x80)
      {
        i += 1;
        return REPLACEMENT;
      }
      cp = (cp << 6) | (b & 0x3F);
    }
    i += len;
    return cp;
  }

  /// @brief Number of codepoints of @p s.
  inline size_t Length(std::string_view s)
  {
    size_t count = 0;
    for (size_t i = 0; i < s.size(); count++)
      Next(s, i);
    return count;
  }

  /**
   * @brief Removes the last codepoint of @p s (all its bytes), e.g. for Backspace.
   *
   * Steps back over the continuation bytes (10xxxxxx), so "é" (2 bytes) is
   * removed whole. Does nothing on an empty string.
   */
  inline void PopBack(std::string& s)
  {
    if (s.empty())
      return;
    size_t i = s.size() - 1;
    while (i > 0 && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80)
      i--;
    s.erase(i);
  }

  /**
   * @brief Byte offset where the codepoint before byte @p i starts (0 if @p i is 0).
   *
   * Steps back over the continuation bytes, so it lands on a codepoint
   * boundary. Use it to move a caret one character to the left.
   */
  inline size_t Prev(std::string_view s, size_t i)
  {
    if (i == 0)
      return 0;
    if (i > s.size())
      i = s.size();
    i--;
    while (i > 0 && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80)
      i--;
    return i;
  }

  /// @brief Byte offset where the codepoint after the one at byte @p i starts (s.size() at the end).
  inline size_t NextOffset(std::string_view s, size_t i)
  {
    if (i >= s.size())
      return s.size();
    Next(s, i);
    return i;
  }

  /// @brief Byte offset where codepoint number @p n starts (s.size() if n >= Length(s)).
  inline size_t ByteOffset(std::string_view s, size_t n)
  {
    size_t i = 0;
    for (size_t count = 0; count < n && i < s.size(); count++)
      Next(s, i);
    return i;
  }
}
