// | -------------------------------
#include "AssetsManager.hpp"
// | -------------------------------
#include "Engine/Render/RImage.hpp"
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <memory>
#include <string>
// | -------------------------------

namespace ENG
{

  void AssetsManager::Load(const std::string& path, const std::string& keyName)
  {
    if (mapImages.contains(keyName)) 
    {
      LOG_INFO(" | << mapImages contains: " + keyName);
      return;
    }

    auto img = std::make_shared<RImage>(path,keyName);
    img->LoadImage();

    mapImages[keyName] = img;
  }

   std::shared_ptr<RImage> AssetsManager::GetTexture(const std::string& keyName)
   {
     if(mapImages.contains(keyName))
       return mapImages[keyName];
    return nullptr;
   }

}
