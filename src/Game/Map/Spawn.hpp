// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Map/CollisionGrid.hpp"
#include "Engine/Map/MapData.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <optional>
#include <string>
#include <vector>
// | -------------------------------

namespace APP
{
  /**
   * @brief Finds where an entity appears on a map (world position of its feet).
   *
   * 1. The first map object whose type or name is @p spawnName (placed in Tiled).
   * 2. Otherwise, the free cell closest to the center of the grid (center of the cell).
   *
   * @param[in] grid      Collision of the map.
   * @param[in] objects   Objects of the map (MapData::m_objects).
   * @param[in] spawnName Type / name of the spawn object.
   * @return The world position, or std::nullopt if every cell is solid.
   */
  std::optional<ENG::Vector2> FindSpawnPoint(const ENG::CollisionGrid& grid,
                                             const std::vector<ENG::MapObjectData>& objects,
                                             const std::string& spawnName = "PlayerSpawn");
}
