// | -------------------------------
#include "Engine.hpp"
// | -------------------------------
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Config.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Layer/GameLayer.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Inputs/PollEvent.hpp"
// | -------------------------------
#include "Engine/Utils/Path.hpp"
#include "SDL3/SDL_error.h"
#include "SDL3_mixer/SDL_mixer.h"
// | -------------------------------
#include <algorithm>
#include <chrono>
#include <memory>
#include <string>
#include <utility>
// | -------------------------------

namespace ENG
{
  Engine::Engine(const EngineConfig& config, std::unique_ptr<GameLayer> layer)
    : eConfig(std::move(config)), game(std::move(layer)) {}

  Engine::~Engine()
  {
    if(game)
    {
        game.reset();
    }
  }

  bool Engine::OnInit(void)
  {  
    auto& r = Render::Get();
    if(r.InitWindowSDLContext(this->eConfig.title, this->eConfig.vW, this->eConfig.vH) == 0)
    {
      LOG_INFO(" | << SDL InitWindows Context Error :" + static_cast<std::string>(SDL_GetError()));
      return false;
    }
    if(!r.InitGLContext())
      LOG_INFO(" | << OpenGL Context NOT created succesfully" + static_cast<std::string>(SDL_GetError()));
    LOG_INFO(" | << SDL InitWindows Context created succesfully" + static_cast<std::string>(SDL_GetError()));
    LOG_INFO(" | << OpenGL Context created succesfully");
    LOG_INFO(" | << PollEvent Engine created succesfully");
    PollEvent::Get().ClearPollEvent();
    LOG_INFO(" | << OpenGL Context created succesfully");
    LOG_INFO(" | << Render Engine created succesfully");
    
    // Init SDL_Mixer
    if(!MIX_Init())
    {
      LOG_FATAL(" | << SDL_Mixer not could Init");
    }
    LOG_INFO(" | << SDL_Mixer created succesfully");

    Services::ProvideAssets(&amgr);
    Services::ProvideScenes(&sm);
    Services::ProvideFonts(&fm);
    Services::ProvideSFX(&sfx);
    Services::ProvideMusic(&music);
    Services::ProvideShaders(&shaders);
    Services::ProvideWorldSaver(&worldSaver);
    LOG_INFO(" | << AssertsManager created Succesfully");
    LOG_INFO(" | << ScenesManager created Succesfully");
    LOG_INFO(" | << FontsManager created Succesfully");
    LOG_INFO(" | << SFXManager created Succesfully");
    LOG_INFO(" | << MusicManager created Succesfully");
    LOG_INFO(" | << ShadersManager created Succesfully");
    LOG_INFO(" | << WorldSaver created Succesfully");

    // Init Game resources for batching
    r.SetScreenSize(this->eConfig.vW, this->eConfig.vH);
    game->OnInit();
    // Init Batcher and RenderContext
    r.InitRenderContext();
    
    return true;
  }

  bool Engine::OnRun(void)
  {
    LOG_INFO(" | << GameEngine Application is Running");

    // Create DeltaTime
    auto frame_ant = std::chrono::high_resolution_clock::now();
    float dt = 0.16666f; 
    
    while (game->IsRunning())
    {
      // Update DeltaTime
      auto frame_act = std::chrono::high_resolution_clock::now();
      dt = std::chrono::duration_cast<std::chrono::duration<float>>(frame_act-frame_ant).count();
      frame_ant = frame_act;
      dt = std::min(dt, 0.05f);
      m_accumulator += dt;

      PollEvent::Get().ProcessPollEvents();

      while (m_accumulator >= Fixed_timestep)
      {
        game->OnUpdateFixed(Fixed_timestep);
        m_accumulator -= Fixed_timestep;
      }

      game->OnInputs(dt);
      game->OnUpdate(dt);
      auto& r = Render::Get();

      float alpha = m_accumulator / Fixed_timestep;
      // Update Render
      r.UpdateRender();
      // Start Rendering
      r.StartDraw();
      {
        ENG::Render::Get().RenderColor(ENG::Color::Gray); 
        game->OnRender(alpha);
      }
      r.EndDraw();
      // Clear Render
      r.ClearRender();
       
      PollEvent::Get().ResetPollEvent();

    }
    LOG_INFO(" | << Application next to close, waiting...");
    return true;
  }

  void Engine::OnDestroy(void)
  {
    LOG_INFO(" | << Destroying Engine, waiting...");
    LOG_INFO(" | << Destroying Game, waiting...");
    game->OnDestroy();
    game.reset();
    LOG_INFO(" | << Clearing all services...");
    Services::Assets().Clear();
    Services::Scenes().Clear();
    Services::Fonts().Clear();
    Services::Music().Clear();
    Services::SFX().Clear();
    Services::Shaders().Clear();
    // Services::WSaver().PreparedData();
    // if(!Services::WSaver().SaveToDisk(Path::Get().DataPath / "data.dt"))
    //   LOG_ERROR(" | << World not been saved correctly");
    // LOG_INFO(" | << World is been saved correctly");
    Services::WSaver().Clear();
    Services::ProvideAssets(nullptr);
    Services::ProvideScenes(nullptr); 
    Services::ProvideFonts(nullptr);
    Services::ProvideMusic(nullptr);
    Services::ProvideSFX(nullptr);
    Services::ProvideShaders(nullptr);
    Services::ProvideWorldSaver(nullptr);

    LOG_INFO(" | << Destroying PollEventBuffer, waiting...");
    PollEvent::Get().ClearPollEvent();
    PollEvent::Get().DestroyPollEvent();
    LOG_INFO(" | << Destroying Render, waiting...");
    Render::Get().DestroyBatch();
    Render::Get().DestroyWindowSDLContext();
    LOG_INFO(" | << OnDestroy finished, members next...");
  }
}
