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
    const float cw = static_cast<float>(atlas.containerWidth);
    const float ch = static_cast<float>(atlas.containerHeight);

    return UVRect{
      glm::vec2(x / cw,         y / ch),
        glm::vec2((x + w) / cw,   (y + h) / ch),
        glm::vec2(static_cast<float>(w), static_cast<float>(h))
    };
  }

  UVRect GetFrameUV(const AtlasData& atlas, const std::string& animName, int frameIndex)
  {
    const AnimationRegion& a = atlas.animations.at(animName);
    const int frame = frameIndex % a.frameCount;   // frameCount > 0, validated on load.
    return RegionToUV(atlas, a.x + frame * a.w, a.y, a.w, a.h);
  }

  UVRect GetTileUV(const AtlasData& atlas, const std::string& tileID)
  {
    const TileRegion& t = atlas.tiles.at(tileID);
    return RegionToUV(atlas, t.x, t.y, t.w, t.h);
  }

  UVRect GetTileUV(const AtlasData& atlas, int tileID)
  {
    const TileRegion& t = atlas.tilesById.at(tileID);
    return RegionToUV(atlas, t.x, t.y, t.w, t.h);
  }

  AtlasData ParseAtlasJSON(const std::string& jsonPath)
  {
    AtlasData data;
    try
    {
      const std::string jsonContent = Path::Get().ReadFileString(jsonPath);
      const nlohmann::json j = nlohmann::json::parse(jsonContent);

      data.name        = j.value("name", std::string{});
      data.texturePath = j.at("texture").get<std::string>();

      //! "tileSize", only for square cells
      //! Irregular like GUI, not need this 3 options
      //! If the JSON doesnt have 'tileSize' | 'tileWidth' | 'tileHeight', they are going to be zero.
      //! Because, the atlas cells are defined by pixels.
      const int ts    = j.value("tileSize", 0);
      data.tileWidth  = j.value("tileWidth",  ts);
      data.tileHeight = j.value("tileHeight", ts);

      if (j.contains("animations"))
      {
        for (const auto& [animName, animJson] : j.at("animations").items())
        {
          const PixelRect r = ReadRect(animJson, data.tileWidth, data.tileHeight);
          data.animations[animName] = {
            r.x, r.y, r.w, r.h,
            animJson.at("frameCount").get<int>()
          };
        }
      }

      if (j.contains("tiles"))
      {
        for (const auto& [tileName, tileJson] : j.at("tiles").items())
        {
          const PixelRect r = ReadRect(tileJson, data.tileWidth, data.tileHeight);
          const TileRegion region{ r.x, r.y, r.w, r.h, tileJson.value("id", -1) };

          data.tiles[tileName] = region;

          // Insert by id, only one.
          if (region.id >= 0 && !data.tilesById.emplace(region.id, region).second)
          {
            LOG_ERROR(" | << Atlas JSON: id de tile duplicado (" +std::to_string(region.id) + ") en '" + tileName + "'");
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
