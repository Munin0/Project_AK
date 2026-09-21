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
#include "SDL3/SDL_error.h"
#include "SDL3_mixer/SDL_mixer.h"
// | -------------------------------
#include <memory>
#include <string>
#include <utility>
// | -------------------------------

namespace ENG
{
  Engine::Engine(EngineConfig& config, std::unique_ptr<GameLayer> layer)
    : m_eConfig(&config), m_game(std::move(layer)) {}

  bool Engine::OnInit(void)
  {  
    auto& r = Render::Get();
    if(r.InitWindowSDLContext(this->m_eConfig->m_title, this->m_eConfig->m_viewWidth, this->m_eConfig->m_viewHeight) == 0)
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

    Services::ProvideAssets(&m_eAssets);
    Services::ProvideScenes(&m_eScenes);
    Services::ProvideFonts(&m_eFont);
    Services::ProvideSFX(&m_eSFX);
    Services::ProvideMusic(&m_eMusic);
    Services::ProvideShaders(&m_eShaders);
    Services::ProvideWorldSaver(&m_eWorldSaver);
    Services::ProvideClockSaver(&m_eClock);
    Services::ProvideCSVManager(&m_eCSV);
    LOG_INFO(" | << AssertsManager created Succesfully");
    LOG_INFO(" | << ScenesManager created Succesfully");
    LOG_INFO(" | << FontsManager created Succesfully");
    LOG_INFO(" | << SFXManager created Succesfully");
    LOG_INFO(" | << MusicManager created Succesfully");
    LOG_INFO(" | << ShadersManager created Succesfully");
    LOG_INFO(" | << WorldSaver created Succesfully");
    LOG_INFO(" | << ClockManager created Succesfully");
    LOG_INFO(" | << CSVManager created Succesfully");
    // Init Game resources for batching
    r.SetScreenSize(this->m_eConfig->m_viewWidth, this->m_eConfig->m_viewHeight);
    // Init Batcher and RenderContext
    r.InitRenderContext(*m_eConfig);
    // Init Game app
    m_game->OnInit();
    return true;
  }

  bool Engine::OnRun(void)
  {
    LOG_INFO(" | << GameEngine Application is Running");

    // Create DeltaTime
    auto clock = Services::Clock();
    while (m_game->IsRunning())
    {
      clock.Tick();
      auto dt = clock.GetDT();
      PollEvent::Get().ProcessPollEvents();

      m_game->OnUpdateFixed(dt);

      m_game->OnInputs(dt);
      m_game->OnUpdate(dt);
      auto& r = Render::Get();

      // Update Render
      r.UpdateRender();
      // Start Rendering
      r.StartDraw();
      {
        ENG::Render::Get().RenderColor(ENG::Color::Gray); 
        m_game->OnRender(dt);
      }
      r.EndDraw();
      // Clear Render
      r.ClearRender();
       
      PollEvent::Get().ResetPollEvent();

    }
    LOG_INFO(" | << Application next to close, waiting...");

    LOG_INFO(" | << [ENGINE] Saving Engine status");
    return true;
  }
  
  void Engine::OnSave(void)
  {
    // Get information for the Engine config
    auto& r = Render::Get();
    this->m_eConfig->m_eFullscreen = r.GetFullscreen();
    this->m_eConfig->m_viewWidth = r.GetScreenSize().x;
    this->m_eConfig->m_viewHeight = r.GetScreenSize().y;
  }

  void Engine::OnDestroy(void)
  {
    LOG_INFO(" | << [ENGINE] Destroying Engine, waiting...");
    LOG_INFO(" | << [ENGINE] Destroying Game, waiting...");
    m_game->OnDestroy();
    m_game.reset();
    LOG_INFO(" | << [ENGINE] Clearing all services...");
    Services::Assets().Clear();
    Services::Scenes().Clear();
    Services::Fonts().Clear();
    Services::Music().Clear();
    Services::SFX().Clear();
    Services::Shaders().Clear();
    Services::Clock().Clear();
    Services::CSV().Clear();
    Services::WSaver().Clear();
    // Services::WSaver().PreparedData();
    // if(!Services::WSaver().SaveToDisk(Path::Get().DataPath / "data.dt"))
    //   LOG_ERROR(" | << World not been saved correctly");
    // LOG_INFO(" | << World is been saved correctly");
    Services::ProvideAssets(nullptr);
    Services::ProvideScenes(nullptr); 
    Services::ProvideFonts(nullptr);
    Services::ProvideMusic(nullptr);
    Services::ProvideSFX(nullptr);
    Services::ProvideShaders(nullptr);
    Services::ProvideWorldSaver(nullptr);
    Services::ProvideClockSaver(nullptr);
    Services::ProvideCSVManager(nullptr);

    LOG_INFO(" | << [ENGINE] Destroying SDL_MIX, waiting...");
    MIX_Quit();
    LOG_INFO(" | << [ENGINE] Destroying PollEventBuffer, waiting...");
    PollEvent::Get().ClearPollEvent();
    PollEvent::Get().DestroyPollEvent();
    LOG_INFO(" | << [ENGINE] Destroying Render, waiting...");
    Render::Get().DestroyBatch();
    Render::Get().DestroyWindowSDLContext();
    LOG_INFO(" | << [ENGINE] OnDestroy finished");
  }
}
