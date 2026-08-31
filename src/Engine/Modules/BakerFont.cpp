#include "BakerFont.hpp"
// | -------------------------------
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
#include "Engine/Modules/LanguageRanges.hpp"
// | -------------------------------
#include "Engine/Extern/stb_truetype.h"
#include "Engine/Extern/stb_image_write.h"
// | -------------------------------
#include "nlohmann/json_fwd.hpp"
#include <nlohmann/json.hpp>
// | -------------------------------
#include <filesystem>
#include <cstddef>
#include <fstream>
#include <ios>
#include <string>
#include <vector>
// | -------------------------------

namespace ENG
{
  BakerFont::BakerFont(const std::string& fontPath)
  {
    auto& p = Path::Get();

    m_path = p.AssetsPath / fontPath;

    LOG_INFO( " | << M_PATH: " + m_path.string());
    m_buffer = LoadFontFile(m_path);
  
    if (m_buffer.empty())
    {
      LOG_ERROR(" | << ERROR: Font buffer is empty, aborting BakerFont init: " + m_path.string());
      m_valid = false;
      return;
    }

    stbtt_fontinfo info;
    if(!stbtt_InitFont(&info, m_buffer.data(), 0))
    {
      LOG_ERROR(" | << ERROR: stbtt_InitFont have a issue");
      return;
    }
    
    m_bitmapWidth   = 1024;
    m_bitmapHeight  = 1024;
    m_lineHeight    = 64;
    m_scale         = stbtt_ScaleForPixelHeight(&info, m_lineHeight);
    m_valid         = true;
  }

  bool BakerFont::Bake(const std::vector<CharRange>& ranges)
  {
    if (!m_valid)
    {
      LOG_ERROR(" | << ERROR: BakerFont is not valid, aborting Bake()");
      return false;
    }

    m_bitmap.resize(m_bitmapWidth * m_bitmapHeight);

    stbtt_pack_context pc;
    if (!stbtt_PackBegin(&pc, m_bitmap.data(), m_bitmapWidth, m_bitmapHeight, 0, 1, nullptr))
    {
      LOG_ERROR(" | << ERROR: stbtt_PackBegin failed");
      return false;
    }

    stbtt_PackSetOversampling(&pc, 2, 2);

    std::vector<stbtt_pack_range> packRanges(ranges.size());
    std::vector<std::vector<stbtt_packedchar>> packedCharsPerRange(ranges.size());

    for (size_t i = 0; i < ranges.size(); ++i)
    {
      packedCharsPerRange[i].resize(ranges[i].numChars);

      packRanges[i] = {};
      packRanges[i].font_size                        = static_cast<float>(m_lineHeight);
      packRanges[i].first_unicode_codepoint_in_range  = ranges[i].firstCodepoint;
      packRanges[i].array_of_unicode_codepoints       = nullptr; // usamos rango continuo, no lista suelta
      packRanges[i].num_chars                         = ranges[i].numChars;
      packRanges[i].chardata_for_range                = packedCharsPerRange[i].data();
    }

    int result = stbtt_PackFontRanges(&pc, m_buffer.data(), 0,
                                     packRanges.data(), static_cast<int>(packRanges.size()));
    stbtt_PackEnd(&pc);

    if (result == 0)
    {
      LOG_ERROR(" | << ERROR: Font atlas didn't fit, try a bigger bitmap size");
      return false;
    }

    m_glyphs.clear();
    for (size_t r = 0; r < ranges.size(); ++r)
    {
      for (int i = 0; i < ranges[r].numChars; ++i)
      {
        const auto& pc_char = packedCharsPerRange[r][i];

        Glyph glyph;
        glyph.codepoint = ranges[r].firstCodepoint + i;
        glyph.u0 = pc_char.x0 / static_cast<float>(m_bitmapWidth);
        glyph.v0 = pc_char.y0 / static_cast<float>(m_bitmapHeight);
        glyph.u1 = pc_char.x1 / static_cast<float>(m_bitmapWidth);
        glyph.v1 = pc_char.y1 / static_cast<float>(m_bitmapHeight);
        glyph.xoff     = pc_char.xoff;
        glyph.yoff     = pc_char.yoff;
        glyph.xoff2    = pc_char.xoff2;
        glyph.yoff2    = pc_char.yoff2;
        glyph.xadvance = pc_char.xadvance;

        m_glyphs.push_back(glyph);
      }
    }

    return true;  
  }

  bool BakerFont::Bake(const std::vector<Language>& languages)
  {
    auto ranges = GetRangesForLanguages(languages);
    return Bake(ranges);
  }

  void BakerFont::SaveToDisk(const std::filesystem::path& outputDir, const std::string& fontName)
  {
    if (m_bitmap.empty() || m_glyphs.empty())
    {
      LOG_ERROR(" | << ERROR: Nothing to save, call Bake() first");
      return;
    }

    std::filesystem::create_directories(outputDir);

    /// Saved like a PNG on grayscale
    std::filesystem::path pngPath = outputDir / (fontName + ".png");

    int strideInBytes = m_bitmapWidth;
    int writeResult = stbi_write_png(
        pngPath.string().c_str(),
        m_bitmapWidth,
        m_bitmapHeight,
        1,
        m_bitmap.data(),
        strideInBytes);

    if (!writeResult)
    {
      LOG_ERROR(" | << ERROR: stbi_write_png failed for: " + pngPath.string());
      return;
    }

    nlohmann::json j;
    j["fontName"]     = fontName;
    j["lineHeight"]   = m_lineHeight;
    j["atlasWidth"]   = m_bitmapWidth;
    j["atlasHeight"]  = m_bitmapHeight;
    j["scale"]        = m_scale;

    nlohmann::json glyphsJson = nlohmann::json::object();
    for (const auto& g : m_glyphs)
    {
      nlohmann::json glyphEntry;
      glyphEntry["u0"] = g.u0;
      glyphEntry["v0"] = g.v0;
      glyphEntry["u1"] = g.u1;
      glyphEntry["v1"] = g.v1;
      glyphEntry["xoff"]     = g.xoff;
      glyphEntry["yoff"]     = g.yoff;
      glyphEntry["xoff2"]    = g.xoff2;
      glyphEntry["yoff2"]    = g.yoff2;
      glyphEntry["xadvance"] = g.xadvance;
      glyphsJson[std::to_string(g.codepoint)] = glyphEntry;
    }
    j["glyphs"] = glyphsJson;

    std::filesystem::path jsonPath = outputDir / (fontName + ".json");
    std::ofstream jsonFile(jsonPath);
    if (!jsonFile.is_open())

    {
      LOG_ERROR(" | << ERROR: Can't open JSON for writing: " + jsonPath.string());
      return;
    }

    jsonFile << j.dump(2);
  }

  std::vector<unsigned char> BakerFont::LoadFontFile(const std::filesystem::path& path)
  {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
      LOG_ERROR(" | << ERROR: File can't be openned: " + path.string());
      return {};
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
    {
      LOG_ERROR(" | << ERROR: File can't be cast to reinterpret_cast<char*> from unsigned char *. ");
      return {};
    }

    return buffer;
  }
}
