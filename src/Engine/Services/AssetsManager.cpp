// | -------------------------------
#include "AssetsManager.hpp"
// | -------------------------------
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Image/ImagePixel.hpp"
#include "Engine/Render/Image/RImage.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
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
    if (mapImages.contains(keyName)) 
    {
      LOG_INFO(" | << mapImages contains: " + keyName);
      return;
    }

    auto img = std::make_shared<RImage>(p.string(),keyName);
    img->LoadImage();

    mapImages[keyName] = img;
  }

  void AssetsManager::LoadAtlas(const std::string& path, const std::string& idkey)
  {
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
    if(mapImages.contains(keyName))
      return mapImages[keyName];
   return nullptr;
  }

  const AtlasData* AssetsManager::GetAtlas(const std::string& key) const
  {
    auto it = m_atlases.find(key);
    if(it == m_atlases.end())
      return nullptr;
    return &it->second;
  }

  void AssetsManager::Clear()
  {
    mapImages.clear();
    m_atlases.clear();
    m_textureArray.Destroy();
  }
}
