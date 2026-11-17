// | -------------------------------
#include "CollisionGrid.hpp"
// | -------------------------------
#include "Engine/Map/MapData.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <cmath>
#include <cstddef>
#include <string>
// | -------------------------------

namespace ENG
{
  CollisionGrid::CollisionGrid(const MapData& map, float scale)
    : m_orientation{map.m_orientation},
      m_cellSize{map.m_tileSize * scale},
      m_width{map.m_width},
      m_height{map.m_height}
  {
    if (m_width > 0 && m_height > 0)
      m_cells.assign(static_cast<size_t>(m_width) * static_cast<size_t>(m_height), 0);
  }

  bool CollisionGrid::AddLayer(const TileLayerData& layer)
  {
    if (layer.m_width != m_width || layer.m_height != m_height ||
        layer.m_tiles.size() != m_cells.size())
    {
      LOG_ERROR(" | << CollisionGrid: layer '" + layer.m_name + "' does not match the grid size");
      return false;
    }

    for (size_t i = 0; i < m_cells.size(); i++)
    {
      if (layer.m_tiles[i] != EMPTY_TILE)
        m_cells[i] = 1;
    }
    return true;
  }

  void CollisionGrid::SetSolid(int col, int row, bool solid)
  {
    if (col < 0 || row < 0 || col >= m_width || row >= m_height)
      return;
    m_cells[static_cast<size_t>(row) * m_width + col] = solid ? 1 : 0;
  }

  bool CollisionGrid::IsSolid(int col, int row) const
  {
    if (col < 0 || row < 0 || col >= m_width || row >= m_height)
      return true;
    return m_cells[static_cast<size_t>(row) * m_width + col] != 0;
  }

  Vector2 CollisionGrid::WorldToGrid(const Vector2& world) const
  {
    float localX = world.x - m_position.x;
    float localY = world.y - m_position.y;

    if (m_orientation == TileOrientation::Isometric)
    {
      const float halfW = m_cellSize.x * 0.5f;
      const float halfH = m_cellSize.y * 0.5f;
      if (halfW == 0.0f || halfH == 0.0f)
        return {0.0f, 0.0f};
      // The tile image of cell (0,0) starts at x = 0, so the top vertex of
      // its diamond is at x = halfW.
      localX -= halfW;
      const float col = (localX / halfW + localY / halfH) * 0.5f;
      const float row = (localY / halfH - localX / halfW) * 0.5f;
      return {col, row};
    }

    if (m_cellSize.x == 0.0f || m_cellSize.y == 0.0f)
      return {0.0f, 0.0f};
    return {localX / m_cellSize.x, localY / m_cellSize.y};
  }

  Vector2 CollisionGrid::GridToWorld(float col, float row) const
  {
    if (m_orientation == TileOrientation::Isometric)
    {
      const float halfW = m_cellSize.x * 0.5f;
      const float halfH = m_cellSize.y * 0.5f;
      return {m_position.x + halfW + (col - row) * halfW, m_position.y + (col + row) * halfH};
    }
    return {m_position.x + col * m_cellSize.x, m_position.y + row * m_cellSize.y};
  }

  bool CollisionGrid::IsBlockedAtWorld(const Vector2& world) const
  {
    const Vector2 grid = WorldToGrid(world);
    return IsSolid(static_cast<int>(std::floor(grid.x)), static_cast<int>(std::floor(grid.y)));
  }
}
