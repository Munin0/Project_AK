// | -------------------------------
#include "AtlasData.hpp"
// | -------------------------------
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
#include "glm/ext/vector_float2.hpp"
#include "nlohmann/json_fwd.hpp"
#include <nlohmann/json.hpp>
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  UVRect GetFrameUV(const AtlasData& atlas, const std::string& animName, int frameIndex)
  {
    const AnimationRegion& region = atlas.animations.at(animName);

    int frame = frameIndex % region.frameCount;
    int actualCol = region.col + frame;
    int actualRow = region.row;
    
    int pixelX = actualCol * atlas.tileSize;
    int pixelY = actualRow * atlas.tileSize;

    glm::vec2 uvMin = {
    (float)pixelX / atlas.containerWidth,
    (float)pixelY / atlas.containerHeight
    };

    glm::vec2 uvMax = {
    (float)(pixelX + atlas.tileSize) / atlas.containerWidth,
    (float)(pixelY + atlas.tileSize) / atlas.containerHeight
    };

    return { uvMin, uvMax };
  }
  
  UVRect GetTileUV(const AtlasData& atlas, const std::string& tileID)
  {
    float ts = static_cast<float>(atlas.tileSize);
    const auto& tile = atlas.tiles.at(tileID);
    glm::vec2 uvMin = glm::vec2(tile.col * ts / atlas.containerWidth, tile.row * ts / atlas.containerHeight);
    glm::vec2 uvMax = glm::vec2((tile.col + 1) * ts / atlas.containerWidth, (tile.row + 1) * ts / atlas.containerHeight);

    return UVRect{uvMin, uvMax};
  }

  UVRect GetTileUV(const AtlasData& atlas, int tileID)
  {
    float ts = static_cast<float>(atlas.tileSize);
    const auto& tile = atlas.tilesById.at(tileID);
    glm::vec2 uvMin = glm::vec2(tile.col * ts / atlas.containerWidth, tile.row * ts / atlas.containerHeight);
    glm::vec2 uvMax = glm::vec2((tile.col + 1) * ts / atlas.containerWidth, (tile.row + 1) * ts / atlas.containerHeight);

    return UVRect{uvMin, uvMax};
  }

  AtlasData ParseAtlasJSON(const std::string& jsonPath)
  {
    AtlasData data;
    try
    {
      std::string jsonContent = Path::Get().ReadFile(jsonPath);
      nlohmann::json j = nlohmann::json::parse(jsonContent);

      data.name        = j.at("name").get<std::string>();
      data.texturePath = j.at("texture").get<std::string>();
      data.tileSize    = j.at("tileSize").get<int>();

      if (j.contains("animations"))
      {
        for (auto& [animName, animJson] : j.at("animations").items())
        {
          data.animations[animName] = {
            animJson.at("col").get<int>(),
            animJson.at("row").get<int>(),
            animJson.at("frameCount").get<int>()
          };
        }
      }

      if (j.contains("tiles"))
      {
        for (auto& [tileID, tileJson] : j.at("tiles").items())
        {
          TileRegion region{
            tileJson.at("col").get<int>(),
            tileJson.at("row").get<int>(),
            tileJson.value("id", 0)
          };
          data.tiles[tileID] = region;
          data.tilesById[region.id] = region;
        }
      }
    }
    catch (const nlohmann::json::exception& e)
    {
      LOG_INFO(" | << Error parsing atlas JSON: " + jsonPath + " - " + e.what());
      throw;
    }
    return data;
  }

}
