// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Map/MapData.hpp"
// | -------------------------------
#include <optional>
#include <string>
// | -------------------------------

namespace IMP
{
  /**
   * @brief Reads maps exported by Tiled (JSON) into ENG::MapData.
   *
   * This is the only place of the project that knows the Tiled format. The
   * engine only sees the resulting MapData, so another editor (LDtk, a
   * custom format, a procedural generator...) only needs its own loader.
   *
   * What it handles:
   * - Orthogonal and isometric maps (finite maps, CSV / plain JSON data).
   * - Several tilesets: each gid is turned into (tileset index, local id),
   *   where the local id is gid - firstgid, i.e. the atlas TileRegion::id.
   * - The flip / rotation bits of the gids (they are cleared; flipped tiles
   *   are drawn unflipped and a warning is logged once).
   * - Group layers (their offset and visibility are applied to children).
   * - Object layers, copied to MapData::m_objects ("class" or "type").
   * - The class of each tile layer (TileLayerData::m_class).
   * - Custom properties of the map, layers and objects, stored as text.
   *
   * It does not decide anything about the layers (draw order, collision...):
   * that is done by SceneMap::Load() with the MapLayerRules of the game.
   *
   * Not supported: infinite maps (chunks) and base64 / compressed data. In
   * that case an error is logged and the layer is skipped.
   *
   * @code
   * auto map = IMP::TiledLoader::Load("Map/Scenes/expIsometrico.json");
   * if (!map) return;
   * @endcode
   */
  class TiledLoader
  {
    public:
      /**
       * @brief Loads a Tiled JSON map.
       *
       * @param[in] mapPath Path of the map, relative to the assets directory.
       * @return The map, or std::nullopt if the file cannot be read or parsed.
       */
      static std::optional<ENG::MapData> Load(const std::string& mapPath);
  };
}
