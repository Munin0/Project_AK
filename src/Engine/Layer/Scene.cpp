// | -------------------------------
#include "Scene.hpp"
// | -------------------------------
#include "Engine/Map/TileMap/TileMap.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
// | -------------------------------
#include <algorithm>
#include <cstdint>
#include <utility>
// | -------------------------------

namespace ENG
{
  void Scene::AddTileMap(TileMap map)
  {
    tileMaps.push_back(std::move(map));
    std::sort(tileMaps.begin(), tileMaps.end(), [](const TileMap& a, const TileMap& b)
    {
      return a.GetLayer() < b.GetLayer();
    });
  }

  void Scene::RenderTileMaps(Batcher& b, uint8_t minLayer, uint8_t maxLayer) const
  {
    for (const auto& map : tileMaps)
    {
      if (map.GetLayer() >= minLayer && map.GetLayer() < maxLayer)
        map.Render(b);
    }
  }
}
