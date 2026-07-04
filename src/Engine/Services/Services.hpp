// | -------------------------------
#pragma once
// | -------------------------------
#include "AssetsManager.hpp"
// | -------------------------------
#include "Engine/Services/MusicManager.hpp"
#include "Engine/Services/SFXManager.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/ShaderManager.hpp"
// | -------------------------------

namespace ENG
{
  class Services
  {
    public:
      static AssetsManager& Assets() {return *assets;}
      static void ProvideAssets(AssetsManager* _)
      {
        assets = _;
        if(assets)
          assets->Init();
      }

      static ScenesManager& Scenes() {return *scenes;}
      static void ProvideScenes(ScenesManager* _) { scenes = _; }

      static ShaderManager& Shaders() {return *shaders;}
      static void ProvideShaders(ShaderManager* _) { shaders = _;}

      static SFXManager& SFX() {return *sfx;}
      static void ProvideSFX(SFXManager* _)
      {
        sfx = _;
        if(sfx)
          sfx->Init();
      }

      static MusicManager& Music() {return *music;}
      static void ProvideMusic(MusicManager* _)
      {
        music = _;
        if(music)
          music->Init();
      }

    private:
      static inline AssetsManager*  assets  = nullptr;
      static inline ShaderManager*  shaders = nullptr;
      static inline ScenesManager*  scenes  = nullptr;
      static inline MusicManager*   music   = nullptr;
      static inline SFXManager*     sfx     = nullptr;
  };
}
