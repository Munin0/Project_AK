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
  /**
   * @brief Location of an animation inside an atlas grid.
   */
  struct AnimationRegion
  {
    /// @brief Position X of the grid cell of the tile.
    int m_x = 0;
    /// @brief Position Y of the grid cell of the tile.
    int m_y = 0;
    /// @brief Width of the grid cell of the tile.
    int m_w = 0;
    /// @brief Height of the grid cell of the tile.
    int m_h = 0;
    /// @brief frameCount of the animation.
    int m_frameCount = 0;
  };

  /**
   * @brief Location of a tile inside an atlas grid.
   */
  struct TileRegion
  {
    /// @brief Position X of the grid cell of the tile.
    int m_x = 0;
    /// @brief Position Y of the grid cell of the tile.
    int m_y = 0;
    /// @brief Width of the grid cell of the tile.
    int m_w = 0;
    /// @brief Height of the grid cell of the tile.
    int m_h = 0;
    /// @brief Numeric ID of the tile. Map importers resolve their own tile ids to this one.
    int m_id = 0;
  };

  /**
   * @brief Description of a texture atlas: its texture, its grid and the
   *        animations and tiles it contains.
   *
   * Some of the members are not loaded from the atlas JSON: they are filled
   * in later, when the atlas is uploaded to the TextureArray with
   * UploadLayer().
   */
  struct AtlasData
  {
    /// @brief Name of the atlas.
    std::string m_name;
    /// @brief Path of the texture image of the atlas.
    std::string m_texturePath;
    /// @brief Actual size of the atlas image, in pixels.
    ///        Populated when UploadLayer() is called.
    int m_atlasWidth = 0, m_atlasHeight = 0;
    /// @brief Width of the tile to Draw
    int m_tileWidth = 0;
    /// @brief Height of the tile to Draw
    int m_tileHeight = 0;
    /// @brief Layer of the TextureArray where the atlas was uploaded.
    ///        Populated when UploadLayer() is called; not loaded from the JSON.
    int m_atlasLayer = -1;
    /// @brief Actual size of the layer in the TextureArray, in pixels.
    ///        It is the base used to normalize the UV coordinates.
    int m_containerWidth = 0, m_containerHeight = 0;
    /// @brief Animations of the atlas, indexed by animation name.
    std::unordered_map<std::string, AnimationRegion> m_animations;
    /// @brief Tiles of the atlas, indexed by tile name.
    std::unordered_map<std::string, TileRegion> m_tiles;
    /// @brief Reverse lookup of the tiles, indexed by TileRegion::id
    ///        (the id used by ENG::TileLayerData::m_tiles).
    std::unordered_map<int, TileRegion> m_tilesById;
  };

  /**
   * @brief Rectangle in texture (UV) coordinates.
   */
  struct UVRect
  {
    /// @brief UV coordinates of one corner of the rectangle (minimum values).
    glm::vec2 m_uvMin;
    /// @brief UV coordinates of the opposite corner of the rectangle (maximum values).
    glm::vec2 m_uvMax;
    /// @brief Size of the region, in pixels (width, height).
    glm::vec2 m_pixelSize;

    UVRect() : m_uvMin(0.0f), m_uvMax(0.0f), m_pixelSize(0.0f) {}
    UVRect(glm::vec2 min, glm::vec2 max, glm::vec2 size)
      : m_uvMin{min}, m_uvMax{max}, m_pixelSize{size} {}
  };

  /**
   * @brief Returns the UV rectangle of a frame of an animation.
   *
   * @param[in] atlas      Atlas that contains the animation.
   * @param[in] animName   Name of the animation, as indexed in AtlasData::animations.
   * @param[in] frameIndex Index of the frame inside the animation, starting at 0.
   * @return The UV rectangle of the frame.
   */
  [[maybe_unused]] UVRect GetFrameUV(const AtlasData& atlas, const std::string& animName, int frameIndex);

  /**
   * @brief Returns the UV rectangle of a tile, looked up by name.
   *
   * @param[in] atlas  Atlas that contains the tile.
   * @param[in] tileID Name of the tile, as indexed in AtlasData::tiles.
   * @return The UV rectangle of the tile.
   */
  [[maybe_unused]] UVRect GetTileUV(const AtlasData& atlas, const std::string& tileID);

  /**
   * @brief Returns the UV rectangle of a tile, looked up by numeric ID.
   *
   * Looks up a tile by its numeric id (TileRegion::id, as used by ENG::TileLayerData).
   *
   * @param[in] atlas  Atlas that contains the tile.
   * @param[in] tileID Numeric ID of the tile, as indexed in AtlasData::tilesById.
   * @return The UV rectangle of the tile.
   */
  [[maybe_unused]] UVRect GetTileUV(const AtlasData& atlas, int tileID);

  /**
   * @brief Reads an atlas description from a JSON file.
   *
   * Only the data stored in the JSON is filled in. The members populated by
   * UploadLayer() (atlas size, layer and container size) are left with their
   * default values.
   *
   * @param[in] jsonPath Path of the atlas JSON file.
   * @return The atlas data read from the file.
   */
  [[maybe_unused]] AtlasData ParseAtlasJSON(const std::string& jsonPath);
}
