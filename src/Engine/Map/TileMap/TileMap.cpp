// | -------------------------------
#include "TileMap.hpp"
// | -------------------------------
#include "Engine/Map/MapData.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <glm/ext/vector_float2.hpp>
// | -------------------------------
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>
// | -------------------------------

namespace ENG
{
  TileMap TileMap::Build(const MapData& map, const TileLayerData& layer, const AtlasData& atlas,
                         float scale)
  {
    TileMap tm;
    tm.m_name        = layer.m_name;
    tm.m_width       = layer.m_width;
    tm.m_height      = layer.m_height;
    tm.m_offset      = layer.m_offset;
    tm.m_scale       = scale;
    tm.m_tileSize    = map.m_tileSize;
    tm.m_orientation = map.m_orientation;
    tm.m_atlasLayer  = atlas.m_atlasLayer;

    const size_t cellCount = static_cast<size_t>(layer.m_width) * static_cast<size_t>(layer.m_height);
    if (layer.m_tiles.size() != cellCount)
    {
      LOG_ERROR(" | << TileMap: layer '" + layer.m_name + "' has " +
                std::to_string(layer.m_tiles.size()) + " tiles, expected " + std::to_string(cellCount));
      tm.m_width = tm.m_height = 0;
      return tm;
    }

    tm.m_cells.assign(cellCount, -1);
    std::unordered_map<int32_t, int16_t> resolved;  // atlas tile id -> palette index
    std::unordered_set<int32_t>          missing;   // ids already reported

    for (size_t i = 0; i < cellCount; i++)
    {
      const int32_t id = layer.m_tiles[i];
      if (id == EMPTY_TILE)
        continue;

      if (auto it = resolved.find(id); it != resolved.end())
      {
        tm.m_cells[i] = it->second;
        continue;
      }

      if (!atlas.m_tilesById.contains(id))
      {
        if (missing.insert(id).second)
          LOG_ERROR(" | << TileMap: atlas '" + atlas.m_name + "' has no tile with id " +
                    std::to_string(id) + " (layer '" + layer.m_name + "')");
        continue;
      }

      if (tm.m_palette.size() >= static_cast<size_t>(std::numeric_limits<int16_t>::max()))
      {
        LOG_ERROR(" | << TileMap: layer '" + layer.m_name + "' uses too many distinct tiles");
        break;
      }

      const UVRect uv = GetTileUV(atlas, id);
      const auto index = static_cast<int16_t>(tm.m_palette.size());
      tm.m_palette.push_back({uv, (uv.m_pixelSize.y - map.m_tileSize.y) * scale});
      resolved[id]  = index;
      tm.m_cells[i] = index;
    }

    return tm;
  }

  void TileMap::Render(Batcher& b) const
  {
    if (m_cells.empty() || m_atlasLayer < 0)
      return;

    // View in world space, as { left, top, right, bottom } (same convention
    // as Camera2D::GetRectCamera(): w/h hold the right/bottom edges).
    Rectangle view;
    if (const Camera2D* cam = b.GetCamera2D())
    {
      view = cam->GetRectCamera();
    }
    else
    {
      const auto screen = Render::Get().GetScreenSize();
      view = Rectangle{.x = 0.0f, .y = 0.0f, .w = screen.x, .h = screen.y};
    }

    if (m_orientation == TileOrientation::Isometric)
      RenderIsometric(b, view);
    else
      RenderOrthogonal(b, view);
  }

  void TileMap::RenderOrthogonal(Batcher& b, const Rectangle& view) const
  {
    const glm::vec2 cell   = m_tileSize * m_scale;
    const glm::vec2 origin = glm::vec2{m_position.x, m_position.y} + m_offset * m_scale;
    if (cell.x <= 0.0f || cell.y <= 0.0f)
      return;

    const int colStart = std::max(static_cast<int>(std::floor((view.x - origin.x) / cell.x)), 0);
    const int colEnd   = std::min(static_cast<int>(std::ceil((view.w - origin.x) / cell.x)), m_width);
    const int rowStart = std::max(static_cast<int>(std::floor((view.y - origin.y) / cell.y)), 0);
    const int rowEnd   = std::min(static_cast<int>(std::ceil((view.h - origin.y) / cell.y)), m_height);

    for (int row = rowStart; row < rowEnd; row++)
    {
      for (int col = colStart; col < colEnd; col++)
      {
        const int index = m_cells[static_cast<size_t>(row) * m_width + col];
        if (index < 0)
          continue;

        const TilePaletteEntry& entry = m_palette[index];
        const glm::vec2 pos  = origin + GridToScreen(col, row);
        const glm::vec2 size = entry.m_uv.m_pixelSize * m_scale;
        b.DrawAtlasSprite(pos, size, m_atlasLayer, entry.m_uv.m_uvMin, entry.m_uv.m_uvMax);
      }
    }
  }

  void TileMap::RenderIsometric(Batcher& b, const Rectangle& view) const
  {
    const glm::vec2 halfCell = m_tileSize * m_scale * 0.5f;
    const glm::vec2 origin   = glm::vec2{m_position.x, m_position.y} + m_offset * m_scale;
    if (halfCell.x <= 0.0f || halfCell.y <= 0.0f)
      return;

    // Inverse of GridToScreen(): sx = (col - row) * halfW, sy = (col + row) * halfH
    auto screenToGrid = [&](glm::vec2 p) -> glm::vec2
    {
      const glm::vec2 local = p - origin;
      return {(local.x / halfCell.x + local.y / halfCell.y) * 0.5f,
              (local.y / halfCell.y - local.x / halfCell.x) * 0.5f};
    };

    const glm::vec2 corners[4] = {
      screenToGrid({view.x, view.y}), screenToGrid({view.w, view.y}),
      screenToGrid({view.x, view.h}), screenToGrid({view.w, view.h}),
    };

    float minCol = corners[0].x, maxCol = corners[0].x;
    float minRow = corners[0].y, maxRow = corners[0].y;
    for (int i = 1; i < 4; i++)
    {
      minCol = std::min(minCol, corners[i].x);
      maxCol = std::max(maxCol, corners[i].x);
      minRow = std::min(minRow, corners[i].y);
      maxRow = std::max(maxRow, corners[i].y);
    }

    // Margin for tiles taller than a cell, which stick out of their diamond.
    const int margin   = 2;
    const int colStart = std::max(static_cast<int>(std::floor(minCol)) - margin, 0);
    const int colEnd   = std::min(static_cast<int>(std::ceil(maxCol)) + margin, m_width);
    const int rowStart = std::max(static_cast<int>(std::floor(minRow)) - margin, 0);
    const int rowEnd   = std::min(static_cast<int>(std::ceil(maxRow)) + margin, m_height);

    // Row-major, top-to-bottom / left-to-right: painter's algorithm.
    for (int row = rowStart; row < rowEnd; row++)
    {
      for (int col = colStart; col < colEnd; col++)
      {
        const int index = m_cells[static_cast<size_t>(row) * m_width + col];
        if (index < 0)
          continue;

        const TilePaletteEntry& entry = m_palette[index];
        const glm::vec2 pos  = origin + GridToScreen(col, row);
        const glm::vec2 size = entry.m_uv.m_pixelSize * m_scale;
        b.DrawAtlasIsometric(m_atlasLayer, entry.m_uv, pos, size, entry.m_offsetY);
      }
    }
  }

  glm::vec2 TileMap::GridToScreen(int col, int row) const
  {
    const glm::vec2 cell = m_tileSize * m_scale;
    if (m_orientation == TileOrientation::Isometric)
      return {(col - row) * cell.x * 0.5f, (col + row) * cell.y * 0.5f};
    return {col * cell.x, row * cell.y};
  }
}
