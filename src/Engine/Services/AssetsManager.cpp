// | -------------------------------
#include "AssetsManager.hpp"
// | -------------------------------
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Image/ImagePixel.hpp"
#include "Engine/Render/Image/RImage.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
#include <filesystem>
#include <memory>
#include <string>
// | -------------------------------

namespace ENG
{
  void AssetsManager::Init()
  {
    m_textureArray.Init(2048, 2048, 16);
  }

  void AssetsManager::Load(const std::string& path, const std::string& keyName)
  {
    auto p = Path::Get().AssetsPath / path;
    if (m_mapImages.contains(keyName)) 
    {
      LOG_ERROR(" | << mapImages contains: " + keyName);
      return;
    }

    auto img = std::make_shared<RImage>(p.string(),keyName);
    img->LoadImage();

    m_mapImages[keyName] = img;
  }

  void AssetsManager::LoadAtlas(const std::string& path, const std::string& idkey)
  {
    if(m_atlases.contains(idkey))
    {
      LOG_ERROR(" | << ERROR: Atlas already exist " + idkey);
      return;
    }

    auto jsonPath = Path::Get().AssetsPath / path;
    AtlasData data = ParseAtlasJSON(jsonPath.string());

    auto texturePath = jsonPath.parent_path() / data.texturePath;
    ImagePixels img = LoadImagePixels(texturePath.string());
    if (!img.data)
      return;

    int layer = m_textureArray.UploadLayer(img.data, img.width, img.height); // RAM → VRAM
    if (layer < 0)
      return;

    data.atlasLayer      = layer;
    data.atlasWidth      = img.width;
    data.atlasHeight     = img.height;
    data.containerWidth  = m_textureArray.GetWidth();
    data.containerHeight = m_textureArray.GetHeight();
    m_atlases[idkey] = data;
  }

  std::shared_ptr<RImage> AssetsManager::GetTexture(const std::string& keyName)
  {
    if(m_mapImages.contains(keyName))
      return m_mapImages[keyName];
   return nullptr;
  }

  const AtlasData* AssetsManager::GetAtlas(const std::string& key) const
  {
    auto it = m_atlases.find(key);
    if(it == m_atlases.end())
      return nullptr;
    return &it->second;
  }

  const AtlasData* AssetsManager::GetAtlasByTexture(const std::string& texturePath) const
  {
    if(texturePath.empty())
      return nullptr;
    const std::string texName = std::filesystem::path(texturePath).filename().string();
    for (const auto& [key, data] : m_atlases)
    {
      if(std::filesystem::path(data.texturePath).filename().string() == texName)
        return &data;
    }
    return nullptr;
  }

  void AssetsManager::Clear()
  {
    m_mapImages.clear();
    m_atlases.clear();
    m_textureArray.Destroy();
  }
}
