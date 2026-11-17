// | -------------------------------
#include "SceneMap.hpp"
// | -------------------------------
#include "Engine/Map/CollisionGrid.hpp"
#include "Engine/Map/MapData.hpp"
#include "Engine/Map/MapLayerRules.hpp"
#include "Engine/Map/TileMap/TileMap.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Services/AssetsManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>
// | -------------------------------

namespace ENG
{
  bool SceneMap::Load(const MapData& map, const std::vector<std::string>& atlasKeys,
                      const MapLayerRules& rules, float scale)
  {
    Clear();
    if (atlasKeys.empty())
    {
      LOG_ERROR(" | << SceneMap::Load: no atlas keys given");
      return false;
    }

    m_collision = CollisionGrid(map, scale);

    for (const TileLayerData& layer : map.m_layers)
    {
      const MapLayerRule rule = ResolveLayerRule(layer, rules);

      if (rule.m_collision)
        m_collision.AddLayer(layer);

      if (!rule.m_render)
        continue;

      const size_t keyIndex = std::min(static_cast<size_t>(layer.m_tileset), atlasKeys.size() - 1);
      const AtlasData* atlas = Services::Assets().GetAtlas(atlasKeys[keyIndex]);
      if (!atlas)
      {
        LOG_ERROR(" | << SceneMap::Load: atlas '" + atlasKeys[keyIndex] + "' is not loaded (layer '" +
                  layer.m_name + "')");
        continue;
      }

      TileMap tileMap = TileMap::Build(map, layer, *atlas, scale);
      tileMap.SetLayer(rule.m_drawLayer);
      Add(std::move(tileMap));
    }
    return true;
  }

  void SceneMap::Clear(void)
  {
    m_tileMaps.clear();
    m_collision = CollisionGrid{};
  }

  void SceneMap::Add(TileMap map)
  {
    m_tileMaps.push_back(std::move(map));
    std::stable_sort(m_tileMaps.begin(), m_tileMaps.end(), [](const TileMap& a, const TileMap& b)
    {
      return a.GetLayer() < b.GetLayer();
    });
  }

  const TileMap* SceneMap::Get(uint8_t layer) const
  {
    for (const auto& map : m_tileMaps)
    {
      if (map.GetLayer() == layer)
        return &map;
    }
    return nullptr;
  }

  void SceneMap::Render(Batcher& b, uint8_t minLayer, uint8_t maxLayer) const
  {
    for (const auto& map : m_tileMaps)
    {
      if (map.GetLayer() >= minLayer && map.GetLayer() < maxLayer)
        map.Render(b);
    }
  }

  bool SceneMap::IsEmpty(void) const
  {
    return m_tileMaps.empty() && m_collision.GetWidth() == 0 && m_collision.GetHeight() == 0;
  }
}
