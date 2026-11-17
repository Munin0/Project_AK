// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Map/MapData.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <glm/ext/vector_float2.hpp>
// | -------------------------------
#include <cstdint>
#include <string>
#include <vector>
// | -------------------------------

namespace ENG
{
  /// @brief Texture coordinates of one distinct tile used by a TileMap.
  struct TilePaletteEntry
  {
    /// @brief UV rectangle (and pixel size) of the tile in the atlas.
    UVRect m_uv;
    /// @brief Vertical offset for tiles taller than the grid cell (isometric).
    float  m_offsetY = 0.0f;
  };

  /**
   * @brief A drawable layer of tiles.
   *
   * A TileMap only knows how to draw a grid of atlas tiles. It is built
   * from format-independent data (MapData / TileLayerData), so it does not
   * know which editor or file the map came from, and it has no collision
   * logic (see CollisionGrid).
   *
   * The texture coordinates are resolved once in Build(), so drawing only
   * walks the visible part of the grid.
   *
   * Usage:
   * @code
   * const ENG::AtlasData* atlas = ENG::Services::Assets().GetAtlas("Cave");
   * ENG::TileMap ground = ENG::TileMap::Build(mapData, mapData.m_layers[0], *atlas);
   * ground.SetLayer(LAYER_GROUND);
   * @endcode
   */
  class TileMap
  {
    public:
      /// @brief Creates an empty TileMap (draws nothing).
      TileMap() = default;

      /**
       * @brief Builds a drawable tile layer.
       *
       * Every tile id of @p layer is looked up in @p atlas
       * (AtlasData::m_tilesById). Ids that are not in the atlas are logged
       * once and drawn as empty cells.
       *
       * @param[in] map   Map the layer belongs to (orientation and tile size).
       * @param[in] layer Layer to build.
       * @param[in] atlas Atlas that holds the tiles of the layer. It must
       *                  already be uploaded (AssetsManager::LoadAtlas()).
       * @param[in] scale Scale applied to the tiles when drawing. Defaults to 1.0.
       * @return The tile map. Its draw-order layer is 0; use SetLayer().
       */
      static TileMap Build(const MapData& map, const TileLayerData& layer, const AtlasData& atlas,
                           float scale = 1.0f);

      /**
       * @brief Draws the part of the tile map that is inside the camera
       *        (or the screen, if the batcher has no camera).
       *
       * @param[in,out] b Batcher that collects the draw data of this frame.
       */
      void Render(Batcher& b) const;

      /// @brief Sets the world position of the map origin.
      void SetPosition(const Vector2& pos) { m_position = pos; }
      /// @brief Returns the world position of the map origin.
      const Vector2& GetPosition(void) const { return m_position; }

      /// @brief Sets the draw-order layer (same convention as Object layers, see Layers.hpp).
      void SetLayer(uint8_t layer) { m_layer = layer; }
      /// @brief Returns the draw-order layer.
      uint8_t GetLayer(void) const { return m_layer; }

      /// @brief Returns the name of the layer this TileMap was built from.
      const std::string& GetName(void) const { return m_name; }
      /// @brief Width of the map, in tiles.
      int GetWidth(void)  const { return m_width; }
      /// @brief Height of the map, in tiles.
      int GetHeight(void) const { return m_height; }
      /// @brief Projection of the map.
      TileOrientation GetOrientation(void) const { return m_orientation; }

    private:
      void RenderIsometric(Batcher& b, const Rectangle& view) const;
      void RenderOrthogonal(Batcher& b, const Rectangle& view) const;
      glm::vec2 GridToScreen(int col, int row) const;

    private:
      /// @brief Name of the layer this TileMap was built from.
      std::string m_name;
      /// @brief Draw-order layer, same convention as Object layers.
      uint8_t m_layer = 0;
      /// @brief Layer of the TextureArray where the atlas is stored. -1 if none.
      int m_atlasLayer = -1;
      /// @brief Size of each grid cell, in pixels (before scale).
      glm::vec2 m_tileSize{0.0f, 0.0f};
      /// @brief Width of the map, in tiles.
      int m_width = 0;
      /// @brief Height of the map, in tiles.
      int m_height = 0;
      /// @brief Scale applied to the tiles when drawing.
      float m_scale = 1.0f;
      /// @brief World position of the map origin.
      Vector2 m_position{0.0f, 0.0f};
      /// @brief Offset of this layer, in pixels (before scale).
      glm::vec2 m_offset{0.0f, 0.0f};
      /// @brief Projection of the map.
      TileOrientation m_orientation = TileOrientation::Orthogonal;
      /// @brief Grid of tiles (width * height): index into m_palette, or -1 if empty.
      std::vector<int16_t> m_cells;
      /// @brief Distinct tiles used by the layer, resolved once in Build().
      std::vector<TilePaletteEntry> m_palette;
  };
}
