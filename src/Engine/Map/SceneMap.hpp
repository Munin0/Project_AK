// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Map/CollisionGrid.hpp"
#include "Engine/Map/MapData.hpp"
#include "Engine/Map/MapLayerRules.hpp"
#include "Engine/Map/TileMap/TileMap.hpp"
#include "Engine/Render/Batching/Batch.hpp"
// | -------------------------------
#include <cstdint>
#include <string>
#include <vector>
// | -------------------------------

namespace ENG
{
  /**
   * @brief A loaded map: its drawable tile layers and its collision grid.
   *
   * Only the scenes that have a map own one (e.g. `ENG::SceneMap m_map;`); a
   * menu does not pay for it. It is built from the format-neutral MapData
   * (any importer, e.g. IMP::TiledLoader) and a MapLayerRules table that says
   * what each layer means.
   *
   * Usage:
   * @code
   * auto data = IMP::TiledLoader::Load("Map/Scenes/exampleIsometrico.json");
   * if (data) m_map.Load(*data, {"I_CaveMap"}, GameLayerRules(), 2.0f);
   *
   * // Render(): interleave the tile layers with the objects
   * m_map.Render(b, LAYER_GROUND, LAYER_PLAYER);   // behind the player
   * ENG::RenderObjects(m_pool, b, LAYER_GROUND, LAYER_UI_BG);
   * m_map.Render(b, LAYER_PLAYER, LAYER_MAX);      // in front of the player
   *
   * // Physics
   * System_MoveOnGrid(player, dir, speed, m_map.GetCollision(), dt);
   * @endcode
   */
  class SceneMap
  {
    public:
      /**
       * @brief Loads a map: its drawable layers and its collisions.
       *
       * Replaces the current content (see Clear()). For every tile layer of
       * @p map, the rule is resolved with ResolveLayerRule() (class, then name,
       * then the layer's own properties):
       * - m_collision: the tiles are added to the collision grid.
       * - m_render:    a TileMap is built and added with Add(), on layer
       *                m_drawLayer.
       *
       * The objects of the map (MapData::m_objects) are not touched: the scene
       * decides what to spawn from them.
       *
       * @param[in] map       Map to load (from any importer).
       * @param[in] atlasKeys Atlas key of each tileset of the map, in order. If
       *                      there are fewer keys than tilesets, the last one is
       *                      used. The atlases must already be loaded.
       * @param[in] rules     Meaning of each layer type, given by the game.
       * @param[in] scale     Draw scale of the tiles. Defaults to 1.0.
       * @return false if nothing could be loaded (no atlas keys).
       */
      bool Load(const MapData& map, const std::vector<std::string>& atlasKeys,
                const MapLayerRules& rules, float scale = 1.0f);

      /// @brief Removes the tile maps and the collision grid.
      void Clear(void);

      /**
       * @brief Adds a tile map, keeping them sorted by TileMap::GetLayer().
       *
       * @param[in] map Tile map to add.
       */
      void Add(TileMap map);

      /**
       * @brief Returns the first tile map with a given draw-order layer.
       *
       * @param[in] layer Draw-order layer (see Layers.hpp).
       * @return The tile map, or nullptr if there is none on that layer.
       */
      const TileMap* Get(uint8_t layer) const;

      /**
       * @brief Draws the tile maps whose layer is in [minLayer, maxLayer).
       *
       * Includes @p minLayer and excludes @p maxLayer, the same rule as
       * RenderObjects(pool, b, min, max), so both can be interleaved. Uses the
       * batcher's active camera.
       *
       * @param[in,out] b        Batcher that collects the draw data of this frame.
       * @param[in]     minLayer First layer to draw (included).
       * @param[in]     maxLayer Layer where the range ends (excluded).
       */
      void Render(Batcher& b, uint8_t minLayer, uint8_t maxLayer) const;

      /// @brief Solid cells of the loaded map. Empty (0 x 0) if none.
      const CollisionGrid& GetCollision(void) const { return m_collision; }

      /// @brief Whether there is nothing loaded (no tile maps and no collision).
      bool IsEmpty(void) const;

    private:
      /// @brief Tile maps, sorted by layer in ascending order.
      std::vector<TileMap> m_tileMaps;

      /// @brief Solid cells of the map. Empty (0 x 0) if none.
      CollisionGrid m_collision;
  };
}
