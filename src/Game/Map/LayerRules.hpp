/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Map/MapLayerRules.hpp"
#include "Engine/Render/Layers.hpp"
/// | ------------------------------------ |

namespace APP
{
  /**
   * @brief Meaning of the map layer types of this game.
   *
   * Each layer of a map is matched by its class in Tiled or, if it has no
   * class, by its name (see ENG::ResolveLayerRule). Add a row here to create
   * a new layer type; a scene can also pass its own table to SceneMap::Load().
   *
   * A layer can still override its row with the custom properties
   * "drawLayer" (int), "collision" (bool) and "render" (bool).
   */
  inline const ENG::MapLayerRules& GameLayerRules()
  {
    static const ENG::MapLayerRules rules = {
      {"Ground",     {.m_drawLayer = LAYER_GROUND}},
      {"Background", {.m_drawLayer = LAYER_GROUND}},
      {"Collision",  {.m_drawLayer = LAYER_COLLISION, .m_collision = true, .m_render = false}},
      {"World",      {.m_drawLayer = LAYER_WORLD,}},
      {"Player",     {.m_drawLayer = LAYER_PLAYER,}},
      {"Middle",     {.m_drawLayer = LAYER_MIDDLE}},
      {"Top",        {.m_drawLayer = LAYER_TOP}},
      {"FX",         {.m_drawLayer = LAYER_FX}},
    };
    return rules;
  }
}
