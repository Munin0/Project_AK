// | -------------------------------
#include "AssetsManager.hpp"
// | -------------------------------
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Image/ImagePixel.hpp"
#include "Engine/Render/Image/RImage.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
#include <exception>
#include <filesystem>
#include <format>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
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

  bool AssetsManager::LoadAtlas(const std::string& path, const std::string& idkey)
  {
    if (m_atlases.contains(idkey))
      return true;

    const auto jsonPath = Path::Get().AssetsPath / path;

    AtlasData data;
    try
    {
      data = ParseAtlasJSON(jsonPath.string());
    }
    catch (const std::exception& e)
    {
      LOG_ERROR(std::format("Atlas '{}': JSON invalido: {}", idkey, e.what()));
      return false;
    }

    const auto texturePath = jsonPath.parent_path() / data.texturePath;
    ImagePixels img = LoadImagePixels(texturePath.string());
    if (!img.data)
    {
      LOG_ERROR(std::format("Atlas '{}': no se pudo cargar la imagen {}", idkey, texturePath.string()));
      return false;
    }

    if (data.tileWidth > 0 && data.tileHeight > 0 &&
        (img.width % data.tileWidth != 0 || img.height % data.tileHeight != 0))
    {
      LOG_ERROR(std::format("Atlas '{}': imagen {}x{} no es multiplo de la celda {}x{}",
            idkey, img.width, img.height, data.tileWidth, data.tileHeight));
    }

    // Una region es valida si es no vacia y cabe dentro de la imagen
    const auto fits = [&](int x, int y, int w, int h)
    {
      return w > 0 && h > 0 &&
        x >= 0 && y >= 0 &&
        x + w <= img.width &&
        y + h <= img.height;
    };

    for (const auto& [animName, a] : data.animations)
    {
      // Los frames avanzan en horizontal: el ancho total es frameCount * w
      if (a.frameCount <= 0 || !fits(a.x, a.y, a.frameCount * a.w, a.h))
      {
        LOG_ERROR(std::format("Atlas '{}': animacion '{}' fuera del atlas (x={}, y={}, frame={}x{}, frames={}, imagen={}x{})",
              idkey, animName, a.x, a.y, a.w, a.h, a.frameCount, img.width, img.height));
        return false;
      }
    }

    for (const auto& [tileName, t] : data.tiles)
    {
      if (!fits(t.x, t.y, t.w, t.h))
      {
        LOG_ERROR(std::format("Atlas '{}': tile '{}' fuera del atlas (x={}, y={}, {}x{}, imagen={}x{})",
              idkey, tileName, t.x, t.y, t.w, t.h, img.width, img.height));
        return false;
      }
    }

    const int layer = m_textureArray.UploadLayer(img.data, img.width, img.height);
    if (layer < 0)
    {
      LOG_ERROR(std::format("Atlas '{}': UploadLayer fallo (TextureArray lleno o imagen mayor al contenedor)", idkey));
      return false;
    }

    data.atlasLayer      = layer;
    data.atlasWidth      = img.width;
    data.atlasHeight     = img.height;
    data.containerWidth  = m_textureArray.GetWidth();
    data.containerHeight = m_textureArray.GetHeight();

    m_atlases.emplace(idkey, std::move(data));
    return true;
  }

  std::shared_ptr<RImage> AssetsManager::GetTexture(const std::string& keyName)
  {
    auto it = m_mapImages.find(std::string_view{keyName});
    if(it == m_mapImages.end())
      return nullptr;
    return it->second;
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
