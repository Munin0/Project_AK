/// | ------------------------------------ |
#include "Demo.hpp"
/// | ------------------------------------ |
#include "Game/Game.hpp"
#include "Game/Systems/Systems.hpp"
/// | ------------------------------------ |
#include "Engine/Layer/Scene.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Component/Component.hpp"
#include "Engine/PollEvent/PollEvent.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Batching/RAPIBatch.hpp"
#include "Engine/Render/Geometry/RGeometry.hpp"
/// | ------------------------------------ |
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_properties.h"
#include "SDL3_mixer/SDL_mixer.h"
/// | ------------------------------------ |
#include <string>
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

  void DemoScene::Init()
  {
    if(isInit)
      return;
   
    /// Load All Assets 
    ENG::Services::Assets().Load("Player/Player.png", "Player");
    // Player creation
    pool.Add(std::make_unique<ENG::Object>("Player"));
    auto player = pool.Get(PLAYER);
    player->SetLayer(LAYER_PLAYER);
    player->GetStats().hp = 100;
    player->GetTransform().position = {100.0f,100.0f};
    player->GetTransform().velocity = {200.0f,200.0f};
    player->AddComponent<ENG::ISprite>("Player",1.0f);
    player->AddComponent<ENG::IBoundingBox>(ENG::Vector2{16.0f,16.0f});

    /// Configuration of the camera
    camera = std::make_unique<ENG::Camera2D>(1280.0f,720.0f);
    camera->SetTarget(player->GetComponent<ENG::IBoundingBox>());
    camera->SetPosition(player->GetTransform().position);
    camera->SetZoom(1.0f);
    ENG::Render::Get().GetBatcher().SetCamera2D(camera.get());
   
    auto rect = std::make_unique<ENG::Rectangle>(0.0f,0.0f,300.0f,100.0f,ENG::Color::Green,ENG::Color::Red, 5.0f); 
    rect->SetLayer(LAYER_BACKGROUND);
    pool.Add(std::move(rect));

    auto line = std::make_unique<ENG::Line>(200.0f,200.0f,500.0f,500.0f, ENG::Color::White);
    line->SetLayer(LAYER_FRONT);
    pool.Add(std::move(line));

    auto poly = std::make_unique<ENG::Polygon>(250.0f,250.0f,5,ENG::Color::Yellow);

    poly->AddPoint(250.0f,200.0f);
    poly->AddPoint(300.0f,250.0f);
    poly->AddPoint(300.0f,300.0f);
    poly->AddPoint(200.0f,300.0f);
    poly->AddPoint(200.0f,250.0f);
    poly->SetLayer(LAYER_FRONT);

    pool.Add(std::move(poly));

    //###############################
    // AUDIO
    // Loading Audios Part
    //###############################
    ENG::Services::SFX().LoadSFX("SFX/Shot_Gun.mp3","Shot", 16); 

    /// option for this music
    SDL_PropertiesID options;
    options = SDL_CreateProperties();
    SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    ENG::Services::Music().LoadMusic("Music/MarineHoloLive.mp3","Marine", options);
    ENG::Services::Music().SetVolume("Marine", 20.0f);

    /// Play Music in the init of the scene
    ENG::Services::Music().PlayMusic("Marine");
  
    isInit = true;
    isRunning = true;

    this->renderQueue = pool.Sort();
  }

  void DemoScene::Inputs(float dt)
  {
    auto& pollEvent = ENG::PollEvent::Get();
    
    if(pollEvent.IsKeyPress(SDL_SCANCODE_P))
    {
      isRunning = false;
      ENG::Services::Scenes().PeddingScene(ENG::SCENE_MENU);
    }

    if(pollEvent.IsKeyPress(SDL_SCANCODE_T))
    {
      ENG::Services::SFX().PlaySFX("Shot");
    }

    if(pollEvent.IsKeyPress(SDL_SCANCODE_KP_PLUS))
    {
      auto& m = ENG::Services::Music();
      auto v = m.GetVolume("Marine");
      v += 10.0f;
      m.SetVolume("Marine",v);
    }
    if(pollEvent.IsKeyPress(SDL_SCANCODE_KP_MINUS))
    {
      auto& m = ENG::Services::Music();
      float v = m.GetVolume("Marine");
      v -= 10.0f;
      m.SetVolume("Marine",v);
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

  void DemoScene::Update(float dt)
  {
    renderQueue = pool.Sort();
    pool.Get(PLAYER)->GetTransform().prev_position = pool.Get(PLAYER)->GetTransform().position;
    System_PlayerMovement(*pool.Get(PLAYER),dt);
    for(auto& o : pool.GetAllIDs())
    {
      pool.Get(o)->Update(dt);
    }
    camera->Update(dt);
    pool.Get(PLAYER)->GetTransform().direction = {0.0f,0.0f};
  }

  void DemoScene::Render(ENG::Batcher& b)
  {
    for(auto& entry : renderQueue)
    {
      pool.Get(entry.id)->Draw(b);
    }

    ENG::Drawer::DrawLine({300.0f,300.0f},{500.0f,300.0f},ENG::Color::Green,2.0f);
    ENG::Drawer::DrawCircle({150.0f,150.0f}, 30.0f, ENG::Color::Green, 64);
    ENG::Drawer::DrawCircleOutLine({150.0f,150.0f}, 30.0f, ENG::Color::Yellow, 64);
  }

  void DemoScene::Destroy()
  {
    pool.Clear();
    renderQueue.clear();
    isInit = false;
    isRunning = false;
  }

  bool DemoScene::IsRunning()
  {
    return isRunning;
  }
}
