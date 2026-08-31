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
  class TileMap
  {
    public:
      TileMap() = default;

      // Parses a map JSON exported by Tiled (relative to assets/) and returns one TileMap per
      // tile layer, in the same bottom-to-top order as the file's "layers" array.
      // atlasKey must reference an atlas already loaded via Services::Assets().LoadAtlas(), whose
      // tile "id"s line up with (gid - firstgid) for the map's (single) tileset.
      // Draw-order layer defaults to 0 for every returned TileMap -- use SetLayer()/GetName() to
      // assign the right LAYER_* value per Tiled layer before handing them to Scene::AddTileMap.
      static std::vector<TileMap> LoadTiledMap(const std::string& atlasKey, const std::string& mapPath,
                                                float scale = 1.0f);
      void Render(Batcher& b) const;

      void SetPosition(const Vector2& pos) { position = pos; }
      const Vector2& GetPosition(void) const { return position; }

      // Draw-order layer, same convention as Object::layer (see LAYER_* in Game.hpp).
      void SetLayer(uint8_t _layer) { layer = _layer; }
      uint8_t GetLayer(void) const { return layer; }

      // Name of the Tiled layer this TileMap was built from (e.g. "Background").
      const std::string& GetName(void) const { return name; }

      int GetWidth(void)  const { return width; }
      int GetHeight(void) const { return height; }

    private:
      std::string name;
      uint8_t layer    = 0;
      int   atlasLayer = -1;
      int   tileSize   = 0;
      int   width      = 0;
      int   height     = 0;
      float scale      = 1.0f;
      Vector2 position{0.0f, 0.0f};

      std::vector<int16_t>    cells;   // width * height; index into the palette; -1 = empty cell
      std::vector<UVRect> palette; // Unique UVs resolved once during LoadTiledMap()
  };
}
