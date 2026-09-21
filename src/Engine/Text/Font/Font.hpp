/// | -------------------------------
#pragma once
/// | -------------------------------
#include "Engine/Render/Image/RImage.hpp"
/// | -------------------------------
#include "nlohmann/json_fwd.hpp"
/// | -------------------------------
#include <memory>
#include <string>
#include <unordered_map>
/// | -------------------------------

namespace ENG
{
  /**
   * @brief Metrics and atlas coordinates of a single character in a Font.
   *
   * A Glyph describes where a character lives inside the font atlas texture
   * and how it must be positioned relative to the text cursor (pen position)
   * when it is drawn.
   */
  struct Glyph
  {
    /// @brief Unicode codepoint this glyph represents.
    int codepoint;
    /// @brief Texture coordinates of the glyph inside the font atlas
    ///        (normalized, [0, 1]). (u0, v0) is the top-left corner and
    ///        (u1, v1) is the bottom-right corner.
    float u0, v0, u1, v1;
    /// @brief Offset of the glyph quad's top-left corner relative to the
    ///        pen position.
    float xoff, yoff;
    /// @brief Offset of the glyph quad's bottom-right corner relative to the
    ///        pen position.
    float xoff2, yoff2;
    /// @brief Horizontal distance to advance the pen after drawing this glyph.
    float xadvance;
  };
  /**
   * @brief Bitmap font loaded from disk.
   *
   * A Font is made of an atlas texture (RImage) that contains the rendered
   * characters, plus a table of Glyph entries that describe where each
   * character is inside that atlas and how it must be laid out. The glyph
   * data is read from a JSON file.
   *
   * Fonts are meant to be used by the text system (see TextAPI) to draw and
   * measure text.
   */
  class Font
  {
    public:
      /**
       * @brief Loads a font by name.
       *
       * Reads the font data (atlas texture and glyph metrics) from disk.
       *
       * @param[in] fontName Name of the font to load.
       */
      Font(const std::string& fontName);
      /// @brief Default destructor.
      ~Font() = default;
      /**
       * @brief Returns the name of the font.
       * @return The font name given at construction.
       */
      std::string GetName(void) const {return this->m_fontName;}
      /**
       * @brief Returns the atlas texture that holds the font's characters.
       * @return Shared pointer to the atlas texture.
       */
      std::shared_ptr<RImage> GetTexture()const {return this->m_texture;}
      /**
       * @brief Returns the vertical distance between two consecutive lines of text.
       * @return Line height of the font.
       */
      float GetLineHeight()const {return this->m_lineHeight;}
      /**
       * @brief Looks up the glyph that corresponds to a character.
       *
       * @param[in] c Unicode codepoint of the character.
       * @return Pointer to the Glyph, or nullptr if the font does not
       *         contain that character. The pointer is owned by the Font and
       *         stays valid for as long as the Font exists.
       */
      const Glyph* GetGlyphs(int c)const;
    private:
      /**
       * @brief Loads the font data from disk.
       *
       * Reads the font's JSON metadata and its atlas texture.
       *
       * @param[in] fontName Name of the font to load.
       * @return true if the font was loaded successfully, false otherwise.
       */
      bool LoadFromDisk(const std::string& fontName);
      /**
       * @brief Parses the font's JSON metadata.
       *
       * Fills the line height and the glyph table from the given JSON.
       *
       * @param[in] j JSON object with the font metadata.
       * @return true if the JSON was valid and parsed successfully, false otherwise.
       */
      bool ParseFontJSON(const nlohmann::json& j);
    private:
      /// @brief Name of the font.
      std::string m_fontName;
      /// @brief Atlas texture that contains the rendered characters.
      std::shared_ptr<RImage> m_texture;
      /// @brief Vertical distance between two consecutive lines of text.
      float m_lineHeight;
      /// @brief Glyph table, indexed by Unicode codepoint.
      std::unordered_map<int, Glyph> m_glyphs;
  };
}
