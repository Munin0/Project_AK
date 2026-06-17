// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/RImage.hpp"
// | -------------------------------
#include <map>
#include <memory>
#include <string>
// | -------------------------------

namespace ENG
{
  class AssetsManager
  {
    public:
      AssetsManager() = default;
      ~AssetsManager() = default;

      void Load(const std::string& path, const std::string& keyName);
      std::shared_ptr<RImage> GetTexture(const std::string& keyName);
      void Clear(void);

    private:
      std::map<std::string, std::shared_ptr<RImage>> mapImages;
  };

}
