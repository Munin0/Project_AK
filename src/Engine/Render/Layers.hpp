// | -------------------------------
#pragma once
// | -------------------------------

/**
 * @file Layers.hpp
 * @brief Draw-order convention shared by objects and tile maps.
 *
 * Lower values are drawn first. Objects (Object::SetLayer) and tile maps
 * (TileMap::SetLayer) use the same scale, so a scene can interleave them
 * (see SceneMap::Render).
 */
#define LAYER_GROUND      0
#define LAYER_COLLISION   1
#define LAYER_WORLD       10
#define LAYER_PLAYER      20
#define LAYER_MIDDLE      25
#define LAYER_TOP         30

#define LAYER_FX          40
#define LAYER_UI_BG       49
#define LAYER_UI          50
#define LAYER_MAX         255
