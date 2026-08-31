/// | -------------------------------
#include "Font.hpp"
/// | -------------------------------
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
#include "Engine/Services/Services.hpp"
/// | -------------------------------
#include "nlohmann/json_fwd.hpp"
#include <exception>
#include <nlohmann/json.hpp>
/// | -------------------------------
#include <fstream>
#include <filesystem>
#include <string>
/// | -------------------------------

namespace ENG
{
  Font::Font(const std::string& fontName)
    : m_fontName{fontName}
  {
    LoadFromDisk(this->m_fontName);
  }

  bool Font::LoadFromDisk(const std::string& fontName)
  {
    auto& p = Path::Get();
    std::filesystem::path jsonPath = p.AssetsPath / ("Font/" + fontName + ".json");
    std::filesystem::path pngPath  = p.AssetsPath / ("Font/" + fontName + ".png");

    Services::Assets().Load("Font/" + fontName + ".png",fontName);
    m_texture = Services::Assets().GetTexture(fontName);
    if(!m_texture->IsValid())
    {
      LOG_ERROR(" | << ERROR: ATLAS font texture failed to load" + pngPath.string());
      return false;
    }

    std::ifstream jsonFile(jsonPath);
    if(!jsonFile.is_open())
    {
      LOG_ERROR(" | << ERROR: JSON file failed to load" + jsonPath.string());
      return false;
    }

    nlohmann::json j;
    try
    {
        jsonFile >> j;
    }
    catch (const nlohmann::json::parse_error& e)
    {
        LOG_ERROR(" | << ERROR: Font JSON parse failed: " + std::string(e.what()));
        return false;
    }

    return ParseFontJSON(j);
  }

  bool Font::ParseFontJSON(const nlohmann::json& j)
  {
    if (!j.contains("glyphs") || !j.contains("lineHeight"))
    {
        LOG_ERROR(" | << ERROR: Font JSON missing required fields (glyphs/lineHeight)");
        return false;
    }

    m_lineHeight = j["lineHeight"].get<float>();
    m_fontName   = j.value("fontName", "unknown");

    m_glyphs.clear();
    m_glyphs.reserve(j["glyphs"].size());

    for (auto& [key, g] : j["glyphs"].items())
    {
        int codepoint;
        try
        {
            codepoint = std::stoi(key);
        }
        catch (const std::exception&)
        {
            LOG_ERROR(" | << ERROR: Invalid glyph codepoint key: " + key);
            continue;
        }

        Glyph glyph;
        glyph.codepoint = codepoint;
        glyph.u0        = g.value("u0", 0.0f);
        glyph.v0        = g.value("v0", 0.0f);
        glyph.u1        = g.value("u1", 0.0f);
        glyph.v1        = g.value("v1", 0.0f);
        glyph.xoff      = g.value("xoff", 0.0f);
        glyph.yoff      = g.value("yoff", 0.0f);
        glyph.xoff2     = g.value("xoff2", 0.0f);
        glyph.yoff2     = g.value("yoff2", 0.0f);
        glyph.xadvance  = g.value("xadvance", 0.0f);

        m_glyphs[codepoint] = glyph;
    }
    if (m_glyphs.empty())
    {
        LOG_ERROR(" | << ERROR: Font JSON parsed with zero glyphs");
        return false;
    }
    LOG_INFO(" | << Font loaded: " + m_fontName + " (" + std::to_string(m_glyphs.size()) + " glyphs)");
    return true;
  }

  const Glyph* Font::GetGlyphs(int c)const
  {
    auto it = m_glyphs.find(c);
    return it != m_glyphs.end() ? &it->second : nullptr;
  }
}
