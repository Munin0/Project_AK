/// | ------------------------------------ |
#include "Demo.hpp"
/// | ------------------------------------ |
#include "Engine/Component/Component.hpp"
#include "Engine/Render/Batching/RAPIBatch.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Render.hpp"
/// | ------------------------------------ |
#include "Engine/Layer/Scene.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Map/TileMap/TileMap.hpp"
#include "Engine/Inputs/PollEvent.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | ------------------------------------ |
#include "Game/Systems/Systems.hpp"
#include "SDL3/SDL_properties.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3_mixer/SDL_mixer.h"
/// | ------------------------------------ |
#include <memory>
#include <string>
#include <utility>
/// | ------------------------------------|
#define PLAYER 1
/// | ------------------------------------|

namespace APP
{
  DemoScene::DemoScene(const std::string& id) : ENG::Scene{}
  {
    sceneID = id;
  }

  void DemoScene::Init()
  {
    if (isInit)
      return;

    // ###############################
    // Objects  
    // Configuration of the Object entities 
    // ###############################
    auto idPlayer = pool.Add(std::make_unique<ENG::Object>("Player"));
    auto objPlayer = pool.Get(idPlayer);
    objPlayer->SetPosition(ENG::Vector2{500,500});
    objPlayer->GetTransform().m_velocity = {100,100};
    objPlayer->AddComponent<ENG::IBoundingBox>(ENG::Vector2{16,16});
    
    // ###############################
    // Tilemap 
    // Configuration of the Tilemaps
    // ###############################
    // Tile map (Tiled JSON export, atlas resolved by tile "id" == gid - firstgid)
    ENG::Services::Assets().LoadAtlas("Atlas/Isometrico/CaveMap.json", "I_CaveMap");
    for (auto& mapLayer : ENG::TileMap::LoadTiledMap("I_CaveMap", "Map/Scenes/expIsometrico.json"))
    {
      AddTileMap(std::move(mapLayer));
    }

    // ###############################
    // Camera  
    // Configuration of the camera
    // ###############################
    auto r = ENG::Render::Get().GetScreenSize();
    cam = std::make_unique<ENG::Camera2D>(r.x, r.y);
    cam->SetTarget(&objPlayer->GetTransform());
    cam->SetPosition(objPlayer->GetPosition());
    cam->SetZoom(1.0f);
    ENG::Render::Get().GetBatcher().SetCamera2D(cam.get());
   
    // ###############################
    //  audio
    //  loading audios part
    // ###############################

    /// option for this music
    SDL_PropertiesID options;
    options = SDL_CreateProperties();
    SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    ENG::Services::Music().LoadMusic("Music/MarineHoloLive.mp3", "Marine", options);
    ENG::Services::Music().SetVolume("Marine", 10.0f);
    ENG::Services::Music().PlayMusic("Marine");

    ENG::Services::SFX().LoadSFX("SFX/Shot_Gun.mp3", "GunShot", 2);
    ENG::Services::SFX().SetVolume("GunShot", 10.0f);
    
    // ###############################
    // Final
    // Final configurations
    // ###############################
    isInit = true;
    isRunning = true;
    this->renderQueue = pool.Sort();
  }

  void DemoScene::Inputs(float dt)
  {
    auto& pollEvent = ENG::PollEvent::Get();

    if (pollEvent.IsKeyPress(SDL_SCANCODE_P))
    {
      isRunning = false;
      ENG::Services::Scenes().PedingScene("MainMenu");
      ENG::Services::Music().StopMusic("Marine");
    }

    if (pollEvent.IsKeyPress(SDL_SCANCODE_F1))
    {
      ENG::Services::SFX().PlaySFX("GunShot");
    }

    if (pollEvent.IsKeyPress(SDL_SCANCODE_KP_PLUS))
    {
      auto& m = ENG::Services::Music();
      auto  v = m.GetVolume("Marine");
      v += 10.0f;
      m.SetVolume("Marine", v);
    }

    if (pollEvent.IsKeyPress(SDL_SCANCODE_KP_MINUS))
    {
      auto& m = ENG::Services::Music();
      float v = m.GetVolume("Marine");
      v -= 10.0f;
      m.SetVolume("Marine", v);
    }

    if (pollEvent.IsKeyDown(SDL_SCANCODE_W))
    {
      pool.Get(PLAYER)->GetTransform().m_direction.y -= 1.0f;
    }
    if (pollEvent.IsKeyDown(SDL_SCANCODE_S))
    {
      pool.Get(PLAYER)->GetTransform().m_direction.y += 1.0f;
    }
    if (pollEvent.IsKeyDown(SDL_SCANCODE_A))
    {
      pool.Get(PLAYER)->GetTransform().m_direction.x -= 1.0f;
    }
    if (pollEvent.IsKeyDown(SDL_SCANCODE_D))
    {
      pool.Get(PLAYER)->GetTransform().m_direction.x += 1.0f;
    }

    // float wheel = pollEvent.GetMouseWheel();
    // if (wheel != 0.0f)
    // {
    //   float zoom = camera->GetZoom() + wheel * 0.1f;
    //   camera->SetZoom(std::clamp(zoom, 2.0f, 3.0f));
    // }
  }

  void DemoScene::Update(float dt)
  {
    for (auto& o : pool.GetAllIDs())
    {
      auto obj = pool.Get(o);
      if(obj->GetName() == "Player")
      {
        System_PlayerMovement(*obj, dt);
      }
      obj->Update(dt);
    }
    cam->Update(dt);
  }

  void DemoScene::UpdateFixed(float dt)
  {
    (void)dt;
  }

  void DemoScene::Render(ENG::Batcher& b)
  {
    // auto camRect = camera->GetRectCamera();
    RenderTileMaps(b, LAYER_GROUND, LAYER_PLAYER);      /// Layer back of the player
    for (auto& entry : renderQueue)
    {
      auto* obj = pool.Get(entry.id);
      obj->Draw(b);
    }
    RenderTileMaps(b, LAYER_PLAYER, LAYER_MAX);    /// Layer front of the player

    ENG::Drawer::DrawRectangle({0,0,100,100},ENG::Color::Blue);
    // ENG::Drawer::DrawCircleOutLine({150.0f, 150.0f}, 30.0f, ENG::Color::Yellow, 64);
    
  }

  void DemoScene::Destroy()
  {
    pool.Clear();
    renderQueue.clear();
    isInit = false;
    isRunning = false;
    tileMaps.clear();
  }

  bool DemoScene::IsRunning()
  {
    return isRunning;
  }
} // namespace APPProperties for the layer of the map
