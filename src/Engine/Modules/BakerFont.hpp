//// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Modules/LanguageRanges.hpp"
#include "Engine/Text/Font/Font.hpp"
// | -------------------------------
#include <filesystem>
#include <string>
#include <vector>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Bakes a font file into a bitmap atlas and glyph data.
   *
   * BakerFont reads a font file, renders the requested characters into a
   * single bitmap (an atlas) and computes the Glyph metrics of each one
   * (atlas coordinates, offsets and advance). The result can be saved to
   * disk so it can later be loaded as a Font, without having to rasterize
   * the font at runtime.
   *
   * It is meant to be used as an offline tool, not during gameplay.
   *
   * Usage:
   * @code
   * ENG::BakerFont baker("fonts/roboto.ttf");
   * if (baker.Bake(ranges))
   *   baker.SaveToDisk("assets/fonts", "roboto");
   * @endcode
   */
  class BakerFont
  {
    public:
      /**
       * @brief Creates a baker for a font file.
       *
       * @param[in] fontPath Path of the font file to bake.
       */
      BakerFont(const std::string& fontPath);

      /// @brief Default destructor.
      ~BakerFont() = default;

      /**
       * @brief Bakes the characters of a list of ranges.
       *
       * @param[in] ranges Ranges of characters to include in the atlas.
       * @return true if the characters were baked successfully, false otherwise.
       */
      bool Bake(const std::vector<CharRange>& ranges);

      /**
       * @brief Bakes the characters needed by a list of languages.
       *
       * @param[in] languages Languages whose characters are included in the atlas.
       * @return true if the characters were baked successfully, false otherwise.
       */
      bool Bake(const std::vector<Language>& languages);

      /**
       * @brief Saves the baked font to disk.
       *
       * Writes the atlas bitmap and the glyph data of the baked font into a
       * directory.
       *
       * @param[in] outputDir Directory where the files are written.
       * @param[in] fontName  Name of the font, used to name the output files.
       */
      void SaveToDisk(const std::filesystem::path& outputDir, const std::string& fontName);

    private:
      /**
       * @brief Reads a whole font file into memory.
       *
       * @param[in] path Path of the font file.
       * @return The bytes of the file.
       */
      std::vector<unsigned char> LoadFontFile(const std::filesystem::path& path);

    private:
      /// @brief Path of the font file.
      std::filesystem::path m_path;

      /// @brief Bytes of the font file.
      std::vector<unsigned char> m_buffer;

      /// @brief Pixels of the baked atlas bitmap.
      std::vector<unsigned char> m_bitmap;

      /// @brief Glyphs of the baked characters.
      std::vector<Glyph> m_glyphs;

      /// @brief Width of the atlas bitmap, in pixels.
      int m_bitmapWidth;

      /// @brief Height of the atlas bitmap, in pixels.
      int m_bitmapHeight;

      /// @brief Distance between two consecutive lines of text.
      int m_lineHeight;

      /// @brief Scale used to render the font at the baked size.
      float m_scale;

      /// @brief Whether the font file was loaded successfully.
      bool m_valid = false;
  };
}
