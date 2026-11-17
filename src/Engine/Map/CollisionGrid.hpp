// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Map/MapData.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <glm/ext/vector_float2.hpp>
// | -------------------------------
#include <cstdint>
#include <vector>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Grid of solid / free cells used for collisions with a map.
   *
   * It is independent of rendering: it can be built from any tile layer
   * (visible or not) and several layers can be merged into one grid.
   * Orthogonal and isometric grids are supported.
   *
   * @code
   * ENG::CollisionGrid grid(mapData);
   * grid.AddLayer(mapData.m_layers[i]);           // every tile of the layer is solid
   * if (grid.IsBlockedAtWorld(point)) { ... }
   * @endcode
   */
  class CollisionGrid
  {
    public:
      /// @brief Creates an empty grid (0 x 0). Every query reports "blocked".
      CollisionGrid() = default;

      /**
       * @brief Creates an all-free grid with the size and projection of a map.
       *
       * @param[in] map   Map that gives the size, tile size and orientation.
       * @param[in] scale Scale used when the map is drawn. Defaults to 1.0.
       */
      explicit CollisionGrid(const MapData& map, float scale = 1.0f);

      /**
       * @brief Marks as solid every cell of @p layer that holds a tile.
       *
       * The layer offset is ignored: the grid is aligned with the map origin.
       *
       * @param[in] layer Layer to merge. Must have the same size as the grid.
       * @return false if the size does not match (nothing is merged).
       */
      bool AddLayer(const TileLayerData& layer);

      /// @brief Marks a single cell as solid or free. Ignored outside the grid.
      void SetSolid(int col, int row, bool solid);

      /**
       * @brief Tells whether the cell (col, row) is solid.
       *
       * Cells outside the grid count as solid, so objects cannot leave the map.
       */
      bool IsSolid(int col, int row) const;

      /**
       * @brief Converts a world-space point into fractional grid coordinates.
       *
       * The integer part is the cell and the fractional part the position
       * inside it.
       */
      Vector2 WorldToGrid(const Vector2& world) const;

      /**
       * @brief Converts fractional grid coordinates into a world-space point.
       *
       * Inverse of WorldToGrid(). (col + 0.5, row + 0.5) is the center of a cell.
       */
      Vector2 GridToWorld(float col, float row) const;

      /// @brief Tells whether the cell under a world-space point is solid.
      bool IsBlockedAtWorld(const Vector2& world) const;

      /// @brief Sets the world position of the grid origin (same as the TileMap).
      void SetPosition(const Vector2& pos) { m_position = pos; }

      /// @brief Width of the grid, in cells.
      int GetWidth()  const { return m_width; }
      /// @brief Height of the grid, in cells.
      int GetHeight() const { return m_height; }
      /// @brief Size of a cell in world units (tile size * scale).
      glm::vec2 GetCellSize() const { return m_cellSize; }
      /// @brief Projection of the grid.
      TileOrientation GetOrientation() const { return m_orientation; }

    private:
      TileOrientation   m_orientation = TileOrientation::Orthogonal;
      glm::vec2         m_cellSize{0.0f, 0.0f};
      Vector2           m_position{0.0f, 0.0f};
      int               m_width  = 0;
      int               m_height = 0;
      /// @brief 1 = solid, 0 = free (width * height, row-major).
      std::vector<uint8_t> m_cells;
  };
}
