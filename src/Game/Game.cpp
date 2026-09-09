/// | ------------------------------------ |
#include "Game.hpp"
/// | ------------------------------------ |
#include "Engine/Inputs/PollEvent.hpp"
#include "Engine/Modules/BakerFont.hpp"
#include "Engine/Modules/LanguageRanges.hpp"
#include "Engine/Render/Batching/RAPIBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Render/Render.hpp"
/// | ------------------------------------ |
#include "Game/Scenes/Demo.hpp"
#include "Game/Scenes/MainMenu.hpp"
/// | ------------------------------------ |
#include "SDL3/SDL_scancode.h"
/// | ------------------------------------ |
#include <memory>
#include <string>
/// | ------------------------------------ |

namespace APP
{
  Game::Game()
  {
    IsAppEnd = false;
  }

  void Game::OnInit(void)
  {
    LOG_INFO(" | << Game application Init Succesfully");

    /// Baking fonts
    // ENG::BakerFont baker("Font/TTF/CabinBold.ttf");
    // baker.Bake( {ENG::Language::English, ENG::Language::Spanish} );
    // baker.SaveToDisk("assets/Font/","CabinBold");
    
    /// Load All Fonts
    ENG::Services::Fonts().LoadFont(std::make_unique<ENG::Font>("PottaOne"));
    ENG::Services::Fonts().LoadFont(std::make_unique<ENG::Font>("Cabin"));
    ENG::Services::Fonts().LoadFont(std::make_unique<ENG::Font>("CabinItalic"));
    /// Load All Atlas 
    // ENG::Services::Assets().Load("Player/Player.png", "Player");
    /// Adding Shaders
    ENG::Services::Shaders().Load("mono.fs","mono.vs","Mono");
    /// Adding Scenes
    ENG::Services::Scenes().AddScene(std::make_unique<MainMenu>(ENG::SCENE_MENU));
    ENG::Services::Scenes().AddScene(std::make_unique<DemoScene>(ENG::SCENE_DEMO));
    /// Start firts scene
    ENG::Services::Scenes().GetCurrent()->Init();
  }

  void Game::OnDestroy(void)
  {
    LOG_INFO(" | << Destroying Game, waiting...");
    IsAppEnd = true;
  }

  bool Game::IsRunning(void)
  {
    return !IsAppEnd;
  }

  void Game::OnInputs(float dt)
  {
    auto& pollEvent  = ENG::PollEvent::Get();
     
    if(pollEvent.IsKeyPress(SDL_SCANCODE_ESCAPE))
    {
      IsAppEnd = !IsAppEnd;
    }
    if(pollEvent.IsKeyPress(SDL_SCANCODE_F3))
      debugDraw = !debugDraw;

    if(pollEvent.IsKeyPress(SDL_SCANCODE_F1))
    {
      auto& tF = ENG::Render::Get()._toggleFullscreen;
      tF = !tF;
    }
    ENG::Services::Scenes().GetCurrent()->Inputs(dt);
  }

  void Game::OnUpdate(float dt)
  {
    auto s_current = ENG::Services::Scenes().GetCurrent();
    // while running the current scene
    if(!s_current->IsRunning())
    {
      s_current->Destroy();
      ENG::Services::Scenes().ChangeScene();
      ENG::Services::Scenes().GetCurrent()->Init();
      return;
    }
    // Update current scene
    s_current->Update(dt);
  }

  void Game::OnUpdateFixed(float dt)
  {
    auto s_current = ENG::Services::Scenes().GetCurrent();
    /// Fixed Update for physics
    s_current->UpdateFixed(dt);
    (void)dt;
  }

  void Game::OnRender(float dt)
  {
    auto& b = ENG::Render::Get().GetBatcher();
    ENG::Services::Scenes().GetCurrent()->Render(b);
    auto screen = ENG::Render::Get().GetScreenSize();

    ENG::Drawer::DrawLine({screen.x / 2,0.0f},{screen.x / 2, screen.y}, ENG::Color::Red);
    ENG::Drawer::DrawLine({ 0.0f/ 2,screen.y/2.0f},{screen.x, screen.y/2.0f}, ENG::Color::Red);
  }
}
