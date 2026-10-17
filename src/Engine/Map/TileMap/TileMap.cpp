// | -------------------------------
#include "TileMap.hpp"
// | -------------------------------
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include "glm/ext/vector_float2.hpp"
// | -------------------------------
#include "nlohmann/json_fwd.hpp"
#include "nlohmann/json.hpp"
// | -------------------------------
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
// | -------------------------------

namespace ENG
{
  std::vector<TileMap> TileMap::LoadTiledMap(const std::string& atlasKey, const std::string& mapPath, float scale)
  {
    std::vector<TileMap> result;

    const AtlasData* atlas = Services::Assets().GetAtlas(atlasKey);
    if (!atlas)
    {
      LOG_ERROR("TileMap: atlas not found: " + atlasKey);
      return result;
    }

    nlohmann::json j;
    try
    {
      auto jsonPath = Path::Get().AssetsPath / mapPath;
      j = nlohmann::json::parse(Path::Get().ReadFile(jsonPath));
    }
    catch (const nlohmann::json::exception& e)
    {
      LOG_ERROR("TileMap: error parsing Tiled map JSON: " + mapPath + " - " + e.what());
      return result;
    }

    // Only a single (possibly embedded) tileset per map is supported: every gid is resolved as
    // (gid - firstgid) against atlasKey's own tile ids.
    int firstGid = 1;
    if (j.contains("tilesets") && !j.at("tilesets").empty())
      firstGid = j.at("tilesets")[0].value("firstgid", 1);

    /// Transform name to layer to draw
    

    /// MapTileSize 
    glm::vec2 mapTileSize = {
      j.value("tilewidth", 0),
      j.value("tileheight", 0)
    };

    std::string orient = j.value("orientation","orthogonal");
    TileOrientation mapOrientation = (orient == "isometric") ? TileOrientation::Isometric : TileOrientation::Orthogonal;

    for (const auto& layerJson : j.at("layers"))
    {
      if (layerJson.value("type", "") != "tilelayer")
        continue;

      TileMap map;
      map.m_name            = layerJson.value("name", "");
      map.m_layer           = map.GetLayerNum(map.m_name);
      map.m_offsetLayer.x = layerJson.value("offsetx",0);
      map.m_offsetLayer.y = layerJson.value("offsety",0);
      map.m_width           = layerJson.at("width").get<int>();
      map.m_height          = layerJson.at("height").get<int>();
      map.m_scale           = scale;
      map.m_atlasLayer      = atlas->atlasLayer;
      map.m_tileSize        = mapTileSize;
      map.m_orientation     = mapOrientation;
      map.SetPosition({0.0f, 0.0f});

      const auto& gids = layerJson.at("data");
      map.m_cells.resize(gids.size());

      std::unordered_map<int, int> resolved; // atlas tile id -> palette index
      for (size_t i = 0; i < gids.size(); i++)
      {
        int gid = gids[i].get<int>();
        if (gid == 0)
        {
          map.m_cells[i] = -1;
          continue;
        }

        int id = gid - firstGid;
        auto it = resolved.find(id);
        if (it != resolved.end())
        {
          map.m_cells[i] = it->second;
          continue;
        }

        try
        {
          int index = static_cast<int>(map.m_palette.size());
          UVRect uv = GetTileUV(*atlas, id);
          float offsetY = (uv.pixelSize.y - mapTileSize.y) * map.m_scale;
          map.m_palette.push_back( {uv, offsetY});
          resolved[id] = index;
          map.m_cells[i] = index;
        }
        catch (const std::out_of_range&)
        {
          LOG_ERROR("TileMap: atlas '" + atlasKey + "' has no tile with id " + std::to_string(id) +
                     " (gid " + std::to_string(gid) + ") referenced by layer '" + map.m_name + "'");
          map.m_cells[i] = -1;
        }
      }

      result.push_back(std::move(map));
    }

    return result;
  }

  void TileMap::Render(Batcher& b) const
  {
    Rectangle rect;
    auto cam = b.GetCamera2D();
    if(cam)
    {
      rect = cam->GetRectCamera();
    }
    else
    {
      auto r = Render::Get().GetScreenSize();
      rect = Rectangle {
        .x = 0.0f,
        .y = 0.0f,
        .w = r.x,
        .h = r.y
      };
    }

    if (m_orientation == TileOrientation::Isometric)
      RenderIsometric(b, rect);
    else
      RenderOrthogonal(b, rect); 
  }

  void TileMap::RenderOrthogonal(Batcher& b, const Rectangle& rect) const
  {
    glm::vec2 cell = m_tileSize * m_scale;

    int colStart = static_cast<int>(std::floor((rect.x - m_position.x) / cell.x));
    int colEnd   = static_cast<int>(std::ceil ((rect.w - m_position.x) / cell.x));
    int rowStart = static_cast<int>(std::floor((rect.y - m_position.y) / cell.y));
    int rowEnd   = static_cast<int>(std::ceil ((rect.h - m_position.y) / cell.y));

    colStart = std::max(colStart, 0);
    rowStart = std::max(rowStart, 0);
    colEnd   = std::min(colEnd, m_width);
    rowEnd   = std::min(rowEnd, m_height);

    for (int row = rowStart; row < rowEnd; row++)
    {
      for (int col = colStart; col < colEnd; col++)
      {
        int index = m_cells[row * m_width + col];
        if (index < 0) continue;

        const TilePaletteEntry& entry = m_palette[index];
        glm::vec2 pos  = { m_position.x + col * cell.x, m_position.y + row * cell.y };
        glm::vec2 size = entry.uv.pixelSize * m_scale;

        b.DrawAtlasSprite(pos, size, m_atlasLayer, entry.uv.uvMin, entry.uv.uvMax);
      }
    }
  }

  void TileMap::RenderIsometric(Batcher& b, const Rectangle& rect) const
  {
    glm::vec2 cell     = m_tileSize * m_scale;
    glm::vec2 halfCell = cell * 0.5f;

    // sx = (col-row)*halfW, sy = (col+row)*halfH
    auto ScreenToGrid = [&](glm::vec2 p) -> glm::vec2
    {
      glm::vec2 local = {p.x - m_position.x, p.y - m_position.y};
      float col = (local.x / halfCell.x + local.y / halfCell.y) * 0.5f;
      float row = (local.y / halfCell.y - local.x / halfCell.x) * 0.5f;
      return { col, row };
    };

    glm::vec2 corners[4] = {
      ScreenToGrid({ rect.x, rect.y }),
      ScreenToGrid({ rect.w, rect.y }),
      ScreenToGrid({ rect.x, rect.h }),
      ScreenToGrid({ rect.w, rect.h }),
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

    const int margin = 1;
    int colStart = std::max(static_cast<int>(std::floor(minCol)) - margin, 0);
    int colEnd   = std::min(static_cast<int>(std::ceil (maxCol)) + margin, m_width);
    int rowStart = std::max(static_cast<int>(std::floor(minRow)) - margin, 0);
    int rowEnd   = std::min(static_cast<int>(std::ceil (maxRow)) + margin, m_height);

    // Row-major, top-to-bottom / left-to-right | painter's algorithm
    for (int row = rowStart; row < rowEnd; row++)
    {
      for (int col = colStart; col < colEnd; col++)
      {
        int index = m_cells[row * m_width + col];
        if (index < 0) continue;

        const TilePaletteEntry& entry = m_palette[index];
        glm::vec2 basePos   = glm::vec2{m_position} + m_offsetLayer + GridToScreen(col,row);
        glm::vec2 size      = entry.uv.pixelSize * m_scale;

        b.DrawAtlasIsometric(m_atlasLayer, entry.uv, basePos, size, entry.offsetY);
      }
    }
  }

  glm::vec2 TileMap::GridToScreen(int col, int row) const
  {
    auto cell = m_tileSize * m_scale;
    switch (m_orientation)
    {
      case TileOrientation::Orthogonal:
        return glm::vec2(col * cell.x, row * cell.y);

      case TileOrientation::Isometric:
        {
          float halfW = cell.x * 0.5f;
          float halfH = cell.y * 0.5f;
          return glm::vec2( (col - row) * halfW,(col + row) * halfH );
        }
    }
    return glm::vec2(0.0f);
  }

  bool TileMap::IsSolid(int col, int row) const
  {
    if (col < 0 || row < 0 || col >= m_width || row >= m_height)
      return true;
    return m_cells[row * m_width + col] >= 0;
  }

  uint8_t TileMap::GetLayerNum(const std::string& name)
  {
    if (name == "Ground") return LAYER_GROUND;
    if (name == "World")      return LAYER_WORLD;
    if (name == "Middle")     return LAYER_MIDDLE;
    if (name == "Top")      return LAYER_TOP;
    if (name == "Collision") return LAYER_COLLISION;
    if (name == "FX")         return LAYER_FX;
    if (name == "UI")         return LAYER_UI;
    return LAYER_GROUND;
  }

  Vector2 TileMap::WorldToGrid(const Vector2& world) const
  {
    glm::vec2 cell = m_tileSize * m_scale;
    glm::vec2 local = glm::vec2(world) - glm::vec2(m_position) - m_offsetLayer;

    if (m_orientation == TileOrientation::Isometric)
    {
      float halfW = cell.x * 0.5f;
      float halfH = cell.y * 0.5f;
      if (halfW == 0.0f || halfH == 0.0f)
        return {0.0f, 0.0f};
      local.x -= halfW;
      float col = (local.x / halfW + local.y / halfH) * 0.5f;
      float row = (local.y / halfH - local.x / halfW) * 0.5f;
      return {col, row};
    }

    if (cell.x == 0.0f || cell.y == 0.0f)
      return {0.0f, 0.0f};
    return {local.x / cell.x, local.y / cell.y};
  }

  bool TileMap::IsCellBlocked(int col, int row) const
  {
    if (col < 0 || row < 0 || col >= m_width || row >= m_height)
      return true;
    return m_cells[row * m_width + col] >= 0;
  }

  bool TileMap::IsBlockedAtWorld(const Vector2& world) const
  {
    Vector2 grid = WorldToGrid(world);
    int col = static_cast<int>(std::floor(grid.x));
    int row = static_cast<int>(std::floor(grid.y));
    return IsCellBlocked(col, row);
  }

}
