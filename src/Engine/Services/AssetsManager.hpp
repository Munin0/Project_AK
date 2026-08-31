// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/Batching/TextureArray.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Image/RImage.hpp"
// | -------------------------------
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
// | -------------------------------

namespace ENG
{
  class AssetsManager
  {
    public:
      AssetsManager() = default;
      ~AssetsManager() = default;

      void Init();
      void Load(const std::string& path, const std::string& keyName);
      void LoadAtlas(const std::string& path, const std::string& key);
      std::shared_ptr<RImage> GetTexture(const std::string& keyName);
      const AtlasData* GetAtlas(const std::string& key) const;
      // Finds a loaded atlas whose texture file name matches the given path (basename only),
      // so Tiled tilesets (which reference an image) can be glued to an already-loaded atlas.
      const AtlasData* GetAtlasByTexture(const std::string& texturePath) const;
      GLuint GetTextureArrayID() const { return m_textureArray.GetID(); }
      void Clear(void);

    private:
      TextureArray m_textureArray;
      std::unordered_map<std::string, AtlasData> m_atlases;
      std::map<std::string, std::shared_ptr<RImage>> m_mapImages;
  };
  
  
  
}
