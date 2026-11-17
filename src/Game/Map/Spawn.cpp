// | -------------------------------
#include "Spawn.hpp"
// | -------------------------------
#include "Engine/Map/CollisionGrid.hpp"
#include "Engine/Map/MapData.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <algorithm>
#include <cstdlib>
#include <optional>
#include <string>
#include <vector>
// | -------------------------------

namespace APP
{
  std::optional<ENG::Vector2> FindSpawnPoint(const ENG::CollisionGrid& grid,
                                             const std::vector<ENG::MapObjectData>& objects,
                                             const std::string& spawnName)
  {
    // 1) A spawn object placed in the editor.
    for (const auto& obj : objects)
      if (obj.m_type == spawnName || obj.m_name == spawnName)
        return grid.GridToWorld(obj.m_gridPos.x, obj.m_gridPos.y);

    // 2) Otherwise, the free cell closest to the center: rings of growing distance.
    const int cx = grid.GetWidth() / 2;
    const int cy = grid.GetHeight() / 2;
    const int maxRadius = std::max(grid.GetWidth(), grid.GetHeight());
    for (int r = 0; r <= maxRadius; r++)
    {
      for (int row = cy - r; row <= cy + r; row++)
      {
        for (int col = cx - r; col <= cx + r; col++)
        {
          if (std::abs(row - cy) != r && std::abs(col - cx) != r)
            continue;  // only the ring at distance r
          if (!grid.IsSolid(col, row))
            return grid.GridToWorld(col + 0.5f, row + 0.5f);
        }
      }
    }
    return std::nullopt;
  }
}
