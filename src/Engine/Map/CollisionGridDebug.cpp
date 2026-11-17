// | -------------------------------
#include "CollisionGridDebug.hpp"
// | -------------------------------
#include "Engine/Map/CollisionGrid.hpp"
#include "Engine/Render/Batching/Drawer.hpp"
#include "Engine/Render/Color/Color.hpp"
// | -------------------------------

namespace ENG
{
  void DrawCollisionGrid(const CollisionGrid& grid, const Color& color)
  {
    for (int row = 0; row < grid.GetHeight(); row++)
    {
      for (int col = 0; col < grid.GetWidth(); col++)
      {
        if (!grid.IsSolid(col, row))
          continue;

        const float c = static_cast<float>(col);
        const float r = static_cast<float>(row);
        Drawer::DrawPolygonOutline({
          grid.GridToWorld(c,        r),
          grid.GridToWorld(c + 1.0f, r),
          grid.GridToWorld(c + 1.0f, r + 1.0f),
          grid.GridToWorld(c,        r + 1.0f),
        }, color);
      }
    }
  }
}
