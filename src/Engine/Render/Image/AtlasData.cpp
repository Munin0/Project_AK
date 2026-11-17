// | -------------------------------
#include "AtlasData.hpp"
// | -------------------------------
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
#include "glm/ext/vector_float2.hpp"
// | -------------------------------
#include "nlohmann/json_fwd.hpp"
#include <nlohmann/json.hpp>
// | -------------------------------
#include <stdexcept>
#include <string>
// | -------------------------------

namespace ENG
{
  struct PixelRect { int x = 0, y = 0, w = 0, h = 0; };

  // Converts a region from JSON to pixels.
  //  - Pixels: "x", "y", "w", "h"
  //  - Grid:   "col", "row", and optional "cols", "rows" (default to 1)
  PixelRect ReadRect(const nlohmann::json& j, int cellW, int cellH)
  {
    if (j.contains("x"))
    {
      return {
        j.at("x").get<int>(), j.at("y").get<int>(),
        j.at("w").get<int>(), j.at("h").get<int>()
      };
    }

    if (cellW <= 0 || cellH <= 0)
      throw std::runtime_error("Region with col/row but not defined on JSON: tileWidth/tileHeight");

    const int col  = j.at("col").get<int>();
    const int row  = j.at("row").get<int>();
    const int cols = j.value("cols", 1);
    const int rows = j.value("rows", 1);

    return { col * cellW, row * cellH, cols * cellW, rows * cellH };
  }

  UVRect RegionToUV(const AtlasData& atlas, int x, int y, int w, int h)
  {
    const float cw = static_cast<float>(atlas.m_containerWidth);
    const float ch = static_cast<float>(atlas.m_containerHeight);

    return UVRect{
      glm::vec2(x / cw,         y / ch),
      glm::vec2((x + w) / cw,   (y + h) / ch),
      glm::vec2(static_cast<float>(w), static_cast<float>(h))
    };
  }

  UVRect GetFrameUV(const AtlasData& atlas, const std::string& animName, int frameIndex)
  {
    auto it = atlas.m_animations.find(animName);
    if(it == atlas.m_animations.end())
    {
      LOG_ERROR("GetFrameUV: frame of animation not found: " + animName);
      return {{0,0},{0,0},{0,0}};
    }
    const AnimationRegion& a = it->second;
    const int frame = frameIndex % a.m_frameCount;   // frameCount > 0, validated on load.
    return RegionToUV(atlas, a.m_x + frame * a.m_w, a.m_y, a.m_w, a.m_h);
  }

  UVRect GetTileUV(const AtlasData& atlas, const std::string& tileID)
  {
    auto it = atlas.m_tiles.find(tileID);
    if(it == atlas.m_tiles.end())
    {
      LOG_ERROR("GetTileUV: tile (ID String) not found");
      return {{0,0},{0,0},{0,0}};
    }
    const auto& t = it->second;
    return RegionToUV(atlas, t.m_x, t.m_y, t.m_w, t.m_h);
  }

  UVRect GetTileUV(const AtlasData& atlas, int tileID)
  {
    auto it = atlas.m_tilesById.find(tileID);
    if(it == atlas.m_tilesById.end())
    {
      LOG_ERROR("GetTileUV: tile (ID Number) not found");
      return {{0,0},{0,0},{0,0}};
    }
    const auto& t = it->second;
    return RegionToUV(atlas, t.m_x, t.m_y, t.m_w, t.m_h);
  }

  AtlasData ParseAtlasJSON(const std::string& jsonPath)
  {
    AtlasData data;
    try
    {
      const std::string jsonContent = Path::Get().ReadFileString(jsonPath);
      // ignore_comments = true: atlas files may carry // comments (e.g. Scenario1)
      const nlohmann::json j = nlohmann::json::parse(jsonContent, nullptr, true, true);

      data.m_name        = j.value("name", std::string{});
      data.m_texturePath = j.at("texture").get<std::string>();

      //! "tileSize", only for square cells
      //! Irregular like GUI, not need this 3 options
      //! If the JSON doesnt have 'tileSize' | 'tileWidth' | 'tileHeight', they are going to be zero.
      //! Because, the atlas cells are defined by pixels.
      const int ts    = j.value("tileSize", 0);
      data.m_tileWidth  = j.value("tileWidth",  ts);
      data.m_tileHeight = j.value("tileHeight", ts);

      if (j.contains("animations"))
      {
        for (const auto& [animName, animJson] : j.at("animations").items())
        {
          const PixelRect r = ReadRect(animJson, data.m_tileWidth, data.m_tileHeight);
          data.m_animations[animName] = {
            r.x, r.y, r.w, r.h,
            animJson.at("frameCount").get<int>()
          };
        }
      }

      if (j.contains("tiles"))
      {
        for (const auto& [tileName, tileJson] : j.at("tiles").items())
        {
          const PixelRect r = ReadRect(tileJson, data.m_tileWidth, data.m_tileHeight);
          const TileRegion region{ r.x, r.y, r.w, r.h, tileJson.value("id", -1) };

          data.m_tiles[tileName] = region;

          // Insert by id, only one.
          if (region.m_id >= 0 && !data.m_tilesById.emplace(region.m_id, region).second)
          {
            LOG_ERROR(" | << Atlas JSON: id de tile duplicado (" +std::to_string(region.m_id) + ") en '" + tileName + "'");
          }
        }
      }
    }
    catch (const nlohmann::json::exception& e)
    {
      throw std::runtime_error("atlas JSON '" + jsonPath + "': " + e.what());
    }
    return data;
  }
}
