// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Utils/RawGeometry.hpp"
// | -------------------------------
#include <glm/ext/vector_float2.hpp>
// | -------------------------------
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
// | -------------------------------

namespace ENG
{
  /// @brief Projection of the tile grid.
  enum class TileOrientation
  {
    Orthogonal,
    Isometric
  };

  /// @brief Free-form key/value properties attached to a layer or an object.
  ///        Values are stored as text ("true", "20", "Player", ...).
  using MapProperties = std::unordered_map<std::string, std::string>;

  /// @brief Value of a tile cell that holds no tile.
  constexpr int32_t EMPTY_TILE = -1;

  /**
   * @brief One layer of tiles, in a format-independent way.
   *
   * The tile ids are already resolved to ids of the atlas of the layer
   * (TileRegion::id), so the engine never sees the ids of the editor
   * that created the map.
   */
  struct TileLayerData
  {
    /// @brief Name of the layer, as given in the editor.
    std::string m_name;
    /// @brief Type (class) of the layer, as given in the editor. Empty if none.
    ///        SceneMap::Load() looks up the layer rules by class first, then by name.
    std::string m_class;
    /// @brief Width of the layer, in tiles.
    int m_width  = 0;
    /// @brief Height of the layer, in tiles.
    int m_height = 0;
    /// @brief Offset of the layer, in pixels.
    glm::vec2 m_offset{0.0f, 0.0f};
    /// @brief Whether the layer is visible (the game may still ignore it).
    bool m_visible = true;
    /// @brief Index into MapData::m_tilesets of the tileset used by the layer.
    int m_tileset = 0;
    /// @brief Atlas tile id of each cell (width * height, row-major), or EMPTY_TILE.
    std::vector<int32_t> m_tiles;
    /// @brief Custom properties of the layer.
    MapProperties m_properties;
  };

  /**
   * @brief An object placed on the map (spawn point, trigger, zone, ...).
   *
   * The engine does not interpret objects: the game decides what each one
   * means by its type, name or properties.
   */
  struct MapObjectData
  {
    /// @brief Name of the object.
    std::string m_name;
    /// @brief Type (or class) of the object, e.g. "PlayerSpawn".
    std::string m_type;
    /// @brief Name of the layer the object belongs to.
    std::string m_layer;
    /// @brief Position and size of the object, in pixels, exactly as stored by
    ///        the editor (its meaning depends on the editor and orientation).
    Rectangle m_rect{0.0f, 0.0f, 0.0f, 0.0f};
    /// @brief Position of the object in fractional grid cells { col, row }.
    ///        Editor-independent: convert it with CollisionGrid::GridToWorld().
    glm::vec2 m_gridPos{0.0f, 0.0f};
    /// @brief Custom properties of the object.
    MapProperties m_properties;
  };

  /// @brief A tileset referenced by the map. The game maps it to an atlas key.
  struct MapTilesetData
  {
    /// @brief Name (or source file) of the tileset.
    std::string m_name;
  };

  /**
   * @brief A whole map, independent of the editor or file format.
   *
   * Importers (for example the Tiled importer in src/Importers) produce a
   * MapData; the engine builds TileMap and CollisionGrid from it.
   */
  struct MapData
  {
    /// @brief Projection of the grid.
    TileOrientation m_orientation = TileOrientation::Orthogonal;
    /// @brief Size of one grid cell, in pixels.
    glm::vec2 m_tileSize{0.0f, 0.0f};
    /// @brief Width of the map, in tiles.
    int m_width  = 0;
    /// @brief Height of the map, in tiles.
    int m_height = 0;
    /// @brief Tilesets referenced by the layers.
    std::vector<MapTilesetData> m_tilesets;
    /// @brief Tile layers, from bottom to top.
    std::vector<TileLayerData> m_layers;
    /// @brief Objects of every object layer.
    std::vector<MapObjectData> m_objects;
    /// @brief Custom properties of the map.
    MapProperties m_properties;
  };

  /**
   * @brief Returns a property, or @p fallback if it is missing.
   */
  inline std::string GetProperty(const MapProperties& props, const std::string& key,
                                 const std::string& fallback = {})
  {
    auto it = props.find(key);
    return it != props.end() ? it->second : fallback;
  }

  /**
   * @brief Tells whether a property exists.
   */
  inline bool HasProperty(const MapProperties& props, const std::string& key)
  {
    return props.contains(key);
  }
}
