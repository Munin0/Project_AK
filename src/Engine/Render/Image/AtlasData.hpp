// | -------------------------------
#pragma once
// | -------------------------------
#include "glm/ext/vector_float2.hpp"
// | -------------------------------
#include <string>
#include <unordered_map>
// | -------------------------------

namespace ENG
{
  struct AnimationRegion
  {
    int col;
    int row;
    int frameCount;
  };

  struct TileRegion
  {
    int col;
    int row;
    int id;
  };

  struct AtlasData
  {
    std::string name;
    std::string texturePath;
    int atlasWidth = 0, atlasHeight = 0; // Actual atlas size in pixels. Populated when UploadLayer() is called.
    int tileSize;                   // Size of each grid cell, in pixels.
    int atlasLayer;                 // Populated when UploadLayer() is called; not loaded from the JSON
    int containerWidth = 0, containerHeight = 0; // Actual layer size in the TextureArray (UV normalization base).
    std::unordered_map<std::string, AnimationRegion> animations;
    std::unordered_map<std::string, TileRegion> tiles;
    std::unordered_map<int, TileRegion> tilesById; // Reverse lookup, keyed by TileRegion::id (Tiled GID - firstgid)
  };

  struct UVRect
  {
    glm::vec2 uvMin;
    glm::vec2 uvMax;
  };

  [[maybe_unused]] UVRect GetFrameUV(const AtlasData& atlas, const std::string& animName, int frameIndex);
  [[maybe_unused]] UVRect GetTileUV(const AtlasData& atlas, const std::string& tileID);
  // Looks up a tile by its numeric id (as used by Tiled: GID - firstgid).
  [[maybe_unused]] UVRect GetTileUV(const AtlasData& atlas, int tileID);
  [[maybe_unused]] AtlasData ParseAtlasJSON(const std::string& jsonPath);
}



