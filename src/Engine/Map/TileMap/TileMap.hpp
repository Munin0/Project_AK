// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <cstdint>
#include <string>
#include <vector>
// | -------------------------------

namespace ENG
{
  /**
   * @brief A tile layer of a map exported from Tiled, ready to be drawn.
   *
   * A TileMap holds the grid of tiles of a single tile layer. Each cell
   * references a tile of an atlas that was uploaded to the TextureArray, and
   * the texture coordinates of the tiles are resolved once when the map is
   * loaded, so drawing only needs to walk the grid.
   *
   * TileMaps are normally created with LoadTiledMap(), which returns one
   * TileMap per tile layer of the Tiled map.
   *
   * Usage:
   * @code
   * std::vector<ENG::TileMap> layers = ENG::TileMap::LoadTiledMap("world_atlas", "maps/level1.json");
   * layers[0].SetLayer(LAYER_BACKGROUND);
   * @endcode
   */
  class TileMap
  {
    public:
      /// @brief Creates an empty TileMap.
      TileMap() = default;

      /**
       * @brief Loads a map exported by Tiled.
       *
       * Parses a map JSON exported by Tiled and returns one TileMap per
       * tile layer, in the same bottom-to-top order as the file's "layers"
       * array.
       *
       * The atlas given by @p atlasKey must already be loaded (see
       * Services::Assets().LoadAtlas()), and the "id" of its tiles must line
       * up with (gid - firstgid) for the map's (single) tileset.
       *
       * The draw-order layer of every returned TileMap defaults to 0. Use
       * SetLayer() and GetName() to assign the right LAYER_* value to each
       * Tiled layer before handing them to Scene::AddTileMap.
       *
       * @param[in] atlasKey Key of the atlas that contains the tiles of the map.
       * @param[in] mapPath  Path of the map JSON file, relative to the assets
       *                     directory.
       * @param[in] scale    Scale applied to the tiles when drawing. Defaults to 1.0.
       * @return One TileMap for each tile layer of the map, from bottom to top.
       */
      static std::vector<TileMap> LoadTiledMap(const std::string& atlasKey, const std::string& mapPath,
                                                float scale = 1.0f);

      /**
       * @brief Draws the tile map.
       *
       * @param[in,out] b Batcher that collects the draw data of this frame.
       */
      void Render(Batcher& b) const;

      /**
       * @brief Sets the position of the tile map in the world.
       *
       * @param[in] pos New position.
       */
      void SetPosition(const Vector2& pos) { position = pos; }

      /**
       * @brief Returns the position of the tile map in the world.
       * @return The current position.
       */
      const Vector2& GetPosition(void) const { return position; }

      /**
       * @brief Returns the layer number that corresponds to a layer name.
       *
       * @param[in] name Name of the layer.
       * @return The layer number.
       */
      uint8_t GetLayerNum(const std::string& name);

      /**
       * @brief Sets the draw-order layer of the tile map.
       *
       * Same convention as Object::layer (see the LAYER_* values in Game.hpp).
       *
       * @param[in] _layer New draw-order layer.
       */
      void SetLayer(uint8_t _layer) { layer = _layer; }

      /**
       * @brief Returns the draw-order layer of the tile map.
       *
       * Same convention as Object::layer (see the LAYER_* values in Game.hpp).
       *
       * @return The draw-order layer.
       */
      uint8_t GetLayer(void) const { return layer; }

      /**
       * @brief Returns the name of the Tiled layer this TileMap was built from.
       * @return The layer name (for example, "Background").
       */
      const std::string& GetName(void) const { return name; }

      /**
       * @brief Returns the width of the map.
       * @return Width, in tiles.
       */
      int GetWidth(void)  const { return width; }

      /**
       * @brief Returns the height of the map.
       * @return Height, in tiles.
       */
      int GetHeight(void) const { return height; }

    private:
      /// @brief Name of the Tiled layer this TileMap was built from.
      std::string name;

      /// @brief Draw-order layer, same convention as Object::layer.
      uint8_t layer    = 0;

      /// @brief Layer of the TextureArray where the atlas of the tiles is
      ///        stored. -1 until it is resolved.
      int   atlasLayer = -1;

      /// @brief Size of each tile, in pixels.
      int   tileSize   = 0;

      /// @brief Width of the map, in tiles.
      int   width      = 0;

      /// @brief Height of the map, in tiles.
      int   height     = 0;

      /// @brief Scale applied to the tiles when drawing.
      float scale      = 1.0f;

      /// @brief Position of the tile map in the world.
      Vector2 position{0.0f, 0.0f};

      /// @brief Grid of tiles (width * height). Each value is an index into
      ///        the palette, or -1 for an empty cell.
      std::vector<int16_t>    cells;

      /// @brief Unique texture coordinates of the tiles, resolved once
      ///        during LoadTiledMap().
      std::vector<UVRect> palette;
  };


}
