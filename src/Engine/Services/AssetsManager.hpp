// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/Batching/TextureArray.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Image/RImage.hpp"
// | -------------------------------
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Loads and stores the textures and atlases used by the game.
   *
   * AssetsManager keeps every loaded texture (RImage) and atlas (AtlasData)
   * under a string key, so they can be requested later by name. It also owns
   * the TextureArray whose OpenGL ID is used by the renderer (see
   * Vertex::ArrayLayer).
   *
   * Usage:
   * @code
   * assets.Init();
   * assets.Load("player.png", "player");
   * std::shared_ptr<ENG::RImage> tex = assets.GetTexture("player");
   * @endcode
   */
  class AssetsManager
  {
    public:
      /// @brief Default constructor.
      AssetsManager() = default;
      /// @brief Default destructor.
      ~AssetsManager() = default;
      /**
       * @brief Initializes the manager.
       *
       * Must be called before loading any asset.
       */
      void Init();
      /**
       * @brief Loads a texture and stores it under a key.
       *
       * @param[in] path    Path of the image file to load.
       * @param[in] keyName Key used to retrieve the texture later with GetTexture().
       */
      void Load(const std::string& path, const std::string& keyName);
      /**
       * @brief Loads an atlas and stores it under a key.
       *
       * @param[in] path Path of the atlas to load.
       * @param[in] key  Key used to retrieve the atlas later with GetAtlas().
       */
      void LoadAtlas(const std::string& path, const std::string& key);
      /**
       * @brief Returns a previously loaded texture.
       *
       * @param[in] keyName Key the texture was loaded with.
       * @return Shared pointer to the texture. It is empty (nullptr) if no
       *         texture exists with that key.
       */
      std::shared_ptr<RImage> GetTexture(const std::string& keyName);
      /**
       * @brief Returns a previously loaded atlas.
       *
       * @param[in] key Key the atlas was loaded with.
       * @return Pointer to the atlas, or nullptr if no atlas exists with that
       *         key. The pointer is owned by the manager and becomes invalid
       *         after Clear().
       */
      const AtlasData* GetAtlas(const std::string& key) const;
      /**
       * @brief Finds a loaded atlas by the file name of its texture.
       *
       * Looks for a loaded atlas whose texture file name matches the given
       * path (basename only). This allows Tiled tilesets, which reference an
       * image, to be linked to an atlas that is already loaded.
       *
       * @param[in] texturePath Path of the texture image. Only the file name
       *                        is compared.
       * @return Pointer to the matching atlas, or nullptr if none matches.
       *         The pointer is owned by the manager and becomes invalid
       *         after Clear().
       */
      const AtlasData* GetAtlasByTexture(const std::string& texturePath) const;
      /**
       * @brief Returns the OpenGL ID of the texture array.
       *
       * @return The OpenGL texture ID of the TextureArray owned by the manager.
       */
      GLuint GetTextureArrayID() const { return m_textureArray.GetID(); }
      /**
       * @brief Releases every loaded asset.
       *
       * Any pointer previously returned by GetAtlas() or GetAtlasByTexture()
       * becomes invalid after this call.
       */
      void Clear(void);
    private:
      /// @brief Texture array reserved for array-based sampling.
      TextureArray m_textureArray;
      /// @brief Loaded atlases, indexed by key.
      std::unordered_map<std::string, AtlasData> m_atlases;
      /// @brief Loaded textures, indexed by key.
      std::map<std::string, std::shared_ptr<RImage>, std::less<>> m_mapImages;
  };
}
