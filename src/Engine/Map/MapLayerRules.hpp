// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Map/MapData.hpp"
// | -------------------------------
#include <cstdint>
#include <string>
#include <unordered_map>
// | -------------------------------

namespace ENG
{
  /**
   * @brief What a map layer means for the scene: draw order, collision and visibility.
   */
  struct MapLayerRule
  {
    /// @brief Draw-order layer of the TileMap (see Layers.hpp).
    uint8_t m_drawLayer = 0;
    /// @brief Whether the tiles of the layer are solid (added to the CollisionGrid).
    bool    m_collision = false;
    /// @brief Whether the layer is drawn. A layer hidden in the editor is never drawn.
    bool    m_render    = true;
  };

  /**
   * @brief Table "layer type -> rule" given by the game when loading a map.
   *
   * The key is the class of the layer (TileLayerData::m_class) or, for
   * layers without class, its name. The engine does not know any layer
   * name: every game (or scene) passes its own table.
   *
   * @code
   * const ENG::MapLayerRules rules = {
   *   {"Background", {.m_drawLayer = LAYER_GROUND}},
   *   {"Collision",  {.m_drawLayer = LAYER_COLLISION, .m_collision = true, .m_render = false}},
   *   {"World",      {.m_drawLayer = LAYER_WORLD,     .m_collision = true}},
   *   {"Top",        {.m_drawLayer = LAYER_TOP}},
   * };
   * @endcode
   */
  using MapLayerRules = std::unordered_map<std::string, MapLayerRule>;

  /**
   * @brief Returns the rule that applies to a layer.
   *
   * 1. Looks up the class of the layer in @p rules, then its name.
   * 2. If there is no rule, uses the default MapLayerRule (logged once per call).
   * 3. The custom properties "drawLayer" (int), "collision" and "render"
   *    (bool) of the layer, if present, override the rule for that layer.
   * 4. A layer hidden in the editor (TileLayerData::m_visible) is not drawn.
   *
   * @param[in] layer Layer to resolve.
   * @param[in] rules Table given by the game.
   * @return The final rule of the layer.
   */
  MapLayerRule ResolveLayerRule(const TileLayerData& layer, const MapLayerRules& rules);
}
