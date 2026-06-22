// | -------------------------------
#pragma once
// | -------------------------------
#include "AssetsManager.hpp"
// | -------------------------------
#include "Engine/Services/MusicManager.hpp"
#include "Engine/Services/SFXManager.hpp"
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

      static SFXManager& SFX() {return *sfx;}
      static void ProvideSFX(SFXManager* _) 
      {
        sfx = _;
        sfx->Init();
      }

      static MusicManager& Music() {return *music;}
      static void ProvideMusic(MusicManager* _)
      {
        music = _;
        music->Init();
      }
    private:
      static inline AssetsManager*  assets  = nullptr;
      static inline SFXManager*     sfx     = nullptr;
      static inline MusicManager*   music   = nullptr;
      static inline ScenesManager*  scenes  = nullptr;
  };
}
