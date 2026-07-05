// | -------------------------------
#include "TileMap.hpp"
// | -------------------------------
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
#include "glm/ext/vector_float2.hpp"
// | -------------------------------
#include "nlohmann/json_fwd.hpp"
#include <nlohmann/json.hpp>
// | -------------------------------
#include <cstddef>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
// | -------------------------------

namespace ENG
{
  std::vector<TileMap> TileMap::LoadTiledMap(const std::string& atlasKey, const std::string& mapPath, float scale)
  {
    std::vector<TileMap> result;

    const AtlasData* atlas = Services::Assets().GetAtlas(atlasKey);
    if (!atlas)
    {
      LOG_ERROR("TileMap: atlas not found: " + atlasKey);
      return result;
    }

    nlohmann::json j;
    try
    {
      auto jsonPath = Path::Get().AssetsPath / mapPath;
      j = nlohmann::json::parse(Path::Get().ReadFile(jsonPath));
    }
    catch (const nlohmann::json::exception& e)
    {
      LOG_ERROR("TileMap: error parsing Tiled map JSON: " + mapPath + " - " + e.what());
      return result;
    }

    // Only a single (possibly embedded) tileset per map is supported: every gid is resolved as
    // (gid - firstgid) against atlasKey's own tile ids.
    int firstGid = 1;
    if (j.contains("tilesets") && !j.at("tilesets").empty())
      firstGid = j.at("tilesets")[0].value("firstgid", 1);

    for (const auto& layerJson : j.at("layers"))
    {
      if (layerJson.value("type", "") != "tilelayer")
        continue;

      TileMap map;
      map.name       = layerJson.value("name", "");
      map.width      = layerJson.at("width").get<int>();
      map.height     = layerJson.at("height").get<int>();
      map.scale      = scale;
      map.atlasLayer = atlas->atlasLayer;
      map.tileSize   = atlas->tileSize;

      const auto& gids = layerJson.at("data");
      map.cells.resize(gids.size());

      std::unordered_map<int, int> resolved; // atlas tile id -> palette index
      for (size_t i = 0; i < gids.size(); i++)
      {
        int gid = gids[i].get<int>();
        if (gid == 0)
        {
          map.cells[i] = -1;
          continue;
        }

        int id = gid - firstGid;
        auto it = resolved.find(id);
        if (it != resolved.end())
        {
          map.cells[i] = it->second;
          continue;
        }

        try
        {
          int index = static_cast<int>(map.palette.size());
          map.palette.push_back(GetTileUV(*atlas, id));
          resolved[id] = index;
          map.cells[i] = index;
        }
        catch (const std::out_of_range&)
        {
          LOG_ERROR("TileMap: atlas '" + atlasKey + "' has no tile with id " + std::to_string(id) +
                     " (gid " + std::to_string(gid) + ") referenced by layer '" + map.name + "'");
          map.cells[i] = -1;
        }
      }

      result.push_back(std::move(map));
    }

    return result;
  }

  void TileMap::Render(Batcher& b) const
  {
    for (int row = 0; row < height; row++)
    {
      for (int col = 0; col < width; col++)
      {
        int index = cells[row * width + col];
        if (index < 0)
          continue;

        const UVRect& uv = palette[index];
        glm::vec2 pos  = {position.x + col * tileSize * scale, position.y + row * tileSize * scale};
        glm::vec2 size = {tileSize * scale, tileSize * scale};

        b.DrawAtlasSprite(pos, size, atlasLayer, uv.uvMin, uv.uvMax);
      }
    }
  }
}
