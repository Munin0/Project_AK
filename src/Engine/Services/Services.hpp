// | -------------------------------
#pragma once
// | -------------------------------
#include "AssetsManager.hpp"
#include "Engine/Services/ScenesManager.hpp"
// | -------------------------------

namespace ENG
{
  class Services
  {
    public:
      static AssetsManager& Assets() {return *assets;}
      static void ProvideAssets(AssetsManager* _) { assets = _; }

      static ScenesManager& Scenes() {return *scenes;}
      static void ProvideScenes(ScenesManager* _) { scenes = _; }
    private:
      static inline AssetsManager* assets = nullptr;
      static inline ScenesManager* scenes = nullptr;
  };
}
