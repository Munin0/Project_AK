#pragma once

#include "Engine/Layer/GameLayer.hpp"
#include "Engine/Utils/Config.hpp"
#include <memory>

#include <string>
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
      EngineConfig eConfig;
      std::unique_ptr<GameLayer> game;
    protected:
  };

}
