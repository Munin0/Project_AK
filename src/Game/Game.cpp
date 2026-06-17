/// | ------------------------------------ |
#include "Game.hpp"
/// | ------------------------------------ |
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/PollEvent/PollEvent.hpp"
#include "Engine/Render/RenderEntry.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Render.hpp"
/// | ------------------------------------ |
#include "Game/Scenes/Demo.hpp"
#include "Game/Scenes/MainMenu.hpp"
/// | ------------------------------------ |
#include "SDL3/SDL_scancode.h"
/// | ------------------------------------ |
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
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
    /// Adding Scenes
    ENG::Services::Scenes().AddScene(std::make_unique<MainMenu>(ENG::SCENE_MENU));
    ENG::Services::Scenes().AddScene(std::make_unique<DemoScene>(ENG::SCENE_DEMO));
    
    /// Load All Assets 
    ENG::Services::Assets().Load(ASSET_PATH"Player/Player.png", "Player");

    /// Start firts scene
    ENG::Services::Scenes().GetCurrent()->Init(pool);
  }

  void Game::OnDestroy(void)
  {
    LOG_INFO(" | << Destroying Game, waiting...");
    IsAppEnd = true;
    pool.Clear();
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
    ENG::Services::Scenes().GetCurrent()->Inputs(dt,pool);
  }

  void Game::OnUpdate(float dt)
  {
    auto s_current = ENG::Services::Scenes().GetCurrent();
    // while running the current scene
    if(!s_current->IsRunning())
    {
      s_current->Destroy(pool);
      ENG::Services::Scenes().ChangeScene();
      ENG::Services::Scenes().GetCurrent()->Init(pool);
      return;
    }

    // Update current scene
    s_current->Update(dt,pool);
  }

  void Game::OnRender(float dt)
  {
    auto& b = ENG::Render::Get().GetBatcher();
    if(!ENG::sortedQueue)
    {
      renderQueue.clear();
      for (ENG::ObjectID id : pool.GetAllIDs())
      {
        auto obj = pool.Get(id);
        renderQueue.push_back({ id, obj->GetLayer() });
      }

      std::sort(renderQueue.begin(), renderQueue.end(),
        [](const ENG::RenderEntry& a, const ENG::RenderEntry& b) {
        return a.layer < b.layer;
      });
      ENG::sortedQueue = true;
    }

    for(auto& entry : renderQueue)
    {
      pool.Get(entry.id)->Draw(b);
    }

    ENG::Services::Scenes().GetCurrent()->Render(b,pool);

    #ifdef DEBUG
    if(ENG::debugDraw)
    {
      for(ENG::ObjectID id : pool.GetAllIDs())
      {
        auto* obj = pool.Get(id);
        auto& t = obj->GetTransform();
        b.DrawQuad(t.position.x, t.position.y, t.size.x, t.size.y, ENG::Color::Green);
      }
    }
    #endif
  }
}
