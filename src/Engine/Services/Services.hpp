// | -------------------------------
#pragma once
// | -------------------------------
#include "AssetsManager.hpp"
// | -------------------------------
#include "Engine/Services/Clock.hpp"
#include "Engine/Services/FontManager.hpp"
#include "Engine/Services/MusicManager.hpp"
#include "Engine/Services/SFXManager.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/ShaderManager.hpp"
#include "Engine/Services/WorldSaver.hpp"
// | -------------------------------

namespace ENG
{
  class Services
  {
    public:
      static AssetsManager& Assets() {return *m_assets;}
      static void ProvideAssets(AssetsManager* _)
      {
        m_assets = _;
        if(m_assets)
          m_assets->Init();
      }

      static FontManager& Fonts() {return *m_fonts;}
      static void ProvideFonts(FontManager* _) {m_fonts = _;}

      static ScenesManager& Scenes() {return *m_scenes;}
      static void ProvideScenes(ScenesManager* _) {m_scenes = _; }

      static ShaderManager& Shaders() {return *m_shaders;}
      static void ProvideShaders(ShaderManager* _) {m_shaders = _;}

      static SFXManager& SFX() {return *m_sfx;}
      static void ProvideSFX(SFXManager* _)
      {
        m_sfx = _;
        if(m_sfx)
          m_sfx->Init();
      }

      static MusicManager& Music() {return *m_music;}
      static void ProvideMusic(MusicManager* _)
      {
        m_music = _;
        if(m_music)
          m_music->Init();
      }

      static WorldSaver& WSaver() {return *m_wSaver;}
      static void ProvideWorldSaver(WorldSaver* _)
      {
        m_wSaver = _;
      }

      static ClockManager& Clock() {return *m_clock;}
      static void ProvideClockSaver(ClockManager* _)
      {
        m_clock = _;
        if(m_clock)
            m_clock->Init();
      }
    private:
      static inline AssetsManager*  m_assets  = nullptr;
      static inline ShaderManager*  m_shaders = nullptr;
      static inline FontManager*    m_fonts   = nullptr;
      static inline ScenesManager*  m_scenes  = nullptr;
      static inline MusicManager*   m_music   = nullptr;
      static inline SFXManager*     m_sfx     = nullptr;
      static inline WorldSaver*     m_wSaver  = nullptr;
      static inline ClockManager*   m_clock   = nullptr;
  };
}
