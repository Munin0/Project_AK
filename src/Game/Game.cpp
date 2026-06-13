/// | ------------------------------------ |
#include "Game.hpp"
/// | ------------------------------------ |
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Render/RenderEntry.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Vector2.hpp" 
#include "Engine/Object/Object.hpp"
#include "Engine/Render/RColor.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Render/RGeometry.hpp"
#include "Engine/Component/Component.hpp"
#include "Engine/PollEvent/PollEvent.hpp"
/// | ------------------------------------ |
#include "Game/Systems/Systems.hpp"
#include "Game/Systems/Systems.hpp"
/// | ------------------------------------ |
#include "SDL3/SDL_scancode.h"
/// | ------------------------------------ |
#include <algorithm>
#include <memory>
#include <string>
#include <utility>
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

    /// Player creation
    pool.Add(std::make_unique<ENG::Object>("Player"));
    auto player = pool.Get(PLAYER);
    player->SetLayer(LAYER_PLAYER);
    player->GetStats().hp = 100;
    player->GetTransform().position = {100,100};
    player->GetTransform().velocity = {200,200};
    ENG::Services::Assets().Load(ASSET_PATH"Player/Player.png", "Player");
    player->AddComponent<ENG::ISprite>("Player",1.0f);
    
    auto rect = std::make_unique<ENG::Rectangle>(0,0,300,100,ENG::Color::Green,ENG::Color::Red, 5.0f);
    rect->SetLayer(LAYER_BACKGROUND);
    pool.Add(std::move(rect));

    auto line = std::make_unique<ENG::Line>(200,200,500,500, ENG::Color::White);
    line->SetLayer(LAYER_WOLRD);
    pool.Add(std::move(line));

    ENG::Vector2 a[3];
    a[0] = {500,100};
    a[1] = {200,300};
    a[2] = {800,300}; 
    auto triangle = std::make_unique<ENG::Triangle>(a[0],a[1],a[2],ENG::Color::Blue, ENG::Color::Yellow);
    triangle->SetLayer(LAYER_WOLRD);
    pool.Add(std::move(triangle));

    auto poly = std::make_unique<ENG::Polygon>(250,250,5,ENG::Color::Yellow);

    poly->AddPoint(250,200);
    poly->AddPoint(300,250);
    poly->AddPoint(300,300);
    poly->AddPoint(200,300);
    poly->AddPoint(200,250);
    poly->SetLayer(LAYER_FRONT);

    pool.Add(std::move(poly));

    ENG::sortedQueue = false;
  }

  void Game::OnDestroy(void)
  {
    IsAppEnd = true;
    pool.Clear();
  }

  bool Game::IsRunning(void)
  {
    return !IsAppEnd;
  }

  void Game::OnInputs(float dt)
  {
    auto& pollEvent = ENG::PollEvent::Get();

    if(pollEvent.IsKeyPress(SDL_SCANCODE_ESCAPE))
    {
      IsAppEnd = true;
    }

    if(pollEvent.IsKeyPress(SDL_SCANCODE_F1))
    {
      auto& tF = ENG::Render::Get()._toggleFullscreen;
      tF = !tF;
    }

    if(pollEvent.IsKeyDown(SDL_SCANCODE_W))
    {
      pool.Get(PLAYER)->GetTransform().direction.y -= 1.0f;
    }
    if(pollEvent.IsKeyDown(SDL_SCANCODE_S))
    {
      pool.Get(PLAYER)->GetTransform().direction.y += 1.0f;
    }
    if(pollEvent.IsKeyDown(SDL_SCANCODE_A))
    {
      pool.Get(PLAYER)->GetTransform().direction.x -= 1.0f;
    }
    if(pollEvent.IsKeyDown(SDL_SCANCODE_D))
    {
      pool.Get(PLAYER)->GetTransform().direction.x += 1.0f;
    }
  }

  void Game::OnUpdate(float dt)
  {
    System_PlayerMovement(*pool.Get(PLAYER),dt);

    // Update las position
    pool.Get(PLAYER)->GetTransform().prev_position = pool.Get(PLAYER)->GetTransform().position;
  }

  void Game::OnRender(float dt)
  {
    auto& b = ENG::Render::Get().GetBatcher();
       
    if(!ENG::sortedQueue)
    {
      renderQueue.clear();
      for (ENG::ObjectID id : pool.GetAllIDs())
        renderQueue.push_back({ id, pool.Get(id)->GetLayer() });

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
  }

}
