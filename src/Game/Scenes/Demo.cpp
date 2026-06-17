/// | ------------------------------------ |
#include "Demo.hpp"
/// | ------------------------------------ |
#include "Engine/Layer/Scene.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/PollEvent/PollEvent.hpp"
#include "Engine/Render/RBatch.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Game/Game.hpp"
/// | ------------------------------------ |
#include "Engine/Render/RColor.hpp"
#include "Engine/Render/RGeometry.hpp"
#include "Engine/Render/RenderEntry.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Component/Component.hpp"
#include "Game/Systems/Systems.hpp"
#include "SDL3/SDL_scancode.h"
/// | ------------------------------------ |
#include <utility>
#include <memory>
/// | ------------------------------------ |

namespace APP
{
  DemoScene::DemoScene(ENG::SceneID id)
    : ENG::Scene{}
  {
    sceneID = id;
  }

  void DemoScene::Init(ENG::ObjectPool& pool)
  {
    if(isInit)
      return;
     
    
    // Player creation
    pool.Add(std::make_unique<ENG::Object>("Player"));
    auto player = pool.Get(PLAYER);
    player->SetLayer(LAYER_PLAYER);
    player->GetStats().hp = 100;
    player->GetTransform().position = {100,100};
    player->GetTransform().velocity = {200,200};
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
    isInit = true;
    isRunning = true;
  }

  void DemoScene::Destroy(ENG::ObjectPool& pool)
  {
    pool.Clear();
    isInit = false;
    isRunning = false;
  }

  bool DemoScene::IsRunning()
  {
    return isRunning;
  }

  void DemoScene::Inputs(float dt, ENG::ObjectPool& pool)
  {
    auto& pollEvent = ENG::PollEvent::Get();
    
    if(pollEvent.IsKeyPress(SDL_SCANCODE_P))
    {
      isRunning = false;
      ENG::Services::Scenes().PeddingScene(ENG::SCENE_MENU);
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

  void DemoScene::Update(float dt, ENG::ObjectPool& pool)
  {
    System_PlayerMovement(*pool.Get(PLAYER),dt);

    // Update las position
    pool.Get(PLAYER)->GetTransform().prev_position = pool.Get(PLAYER)->GetTransform().position;
  }

  void DemoScene::Render(ENG::Batcher& b, ENG::ObjectPool& pool)
  { }

}
