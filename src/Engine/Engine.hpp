// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Layer/GameLayer.hpp"
#include "Engine/Services/AssetsManager.hpp"
#include "Engine/Services/CSVManager.hpp"
#include "Engine/Services/ClockManager.hpp"
#include "Engine/Services/FontManager.hpp"
#include "Engine/Services/MusicManager.hpp"
#include "Engine/Services/SFXManager.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/ShaderManager.hpp"
#include "Engine/Services/WorldSaver.hpp"
#include "Engine/Utils/Config.hpp"
// | -------------------------------
#include <memory>
#include <string>
// | -------------------------------

namespace ENG
{
  class Engine
  {
    public:
      Engine(const EngineConfig& eConfig, std::unique_ptr<GameLayer> layer);
      ~Engine();

      bool OnInit(void);
      bool OnRun(void);
      void OnDestroy(void); 
    private:
      std::string wTitle;
      int wWidth;
      int wHeight;
    private:
      static constexpr float Fixed_timestep = 1.f / 60.f;
      float m_accumulator = 0.0f;
      EngineConfig eConfig;
      std::unique_ptr<GameLayer> game;
      
      /// Services
      AssetsManager amgr;
      ScenesManager sm;
      FontManager fm;
      SFXManager sfx;
      MusicManager music;
      ShaderManager shaders;
      WorldSaver worldSaver;
      ClockManager clock;
      CSVManager csv;
    protected:
  };
}
