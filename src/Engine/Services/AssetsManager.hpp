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
      GLuint GetTextureArrayID() const { return m_textureArray.GetID(); }
      void Clear(void);

    private:
      TextureArray m_textureArray;
      std::unordered_map<std::string, AtlasData> m_atlases;
      std::map<std::string, std::shared_ptr<RImage>> mapImages;
  };
  
  
  
}
