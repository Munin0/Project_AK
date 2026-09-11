/// | ------------------------------------ |
#include "Demo.hpp"
/// | ------------------------------------ |
#include "Game/Game.hpp"
/// | ------------------------------------ |
#include "Engine/Layer/Scene.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Map/TileMap/TileMap.hpp"
#include "Engine/Inputs/PollEvent.hpp"
#include "Engine/Render/Batching/RAPIBatch.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Geometry/RGeometry.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | ------------------------------------ |
#include "SDL3/SDL_properties.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3_mixer/SDL_mixer.h"
/// | ------------------------------------ |
#include <algorithm>
#include <memory>
#include <string>
#include <utility>
/// | ------------------------------------ |

namespace APP
{
  DemoScene::DemoScene(ENG::SceneID id) : ENG::Scene{}
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
    
    // ###############################
    // Tilemap 
    // Configuration of the Tilemaps
    // ###############################
    // Tile map (Tiled JSON export, atlas resolved by tile "id" == gid - firstgid)
    ENG::Services::Assets().LoadAtlas("Atlas/Scenario1/atlasInfo.json", "Scenario1");
    for (auto& mapLayer : ENG::TileMap::LoadTiledMap("Scenario1", "Map/Scenes/Escena1.json"))
    {
      mapLayer.SetLayer(mapLayer.GetLayerNum(mapLayer.GetName()));
      mapLayer.SetPosition({0.0f, 0.0f});
      AddTileMap(std::move(mapLayer));
    }
    // ###############################
    // Camera  
    // Configuration of the camera
    // ###############################
    // camera = std::make_unique<ENG::Camera2D>(1280.0f, 720.0f);
    // camera->SetTarget(player->GetSolidBox());
    // camera->SetPosition(player->GetTransform().position);
    // camera->SetZoom(2.0f);
    // ENG::Render::Get().GetBatcher().SetCamera2D(camera.get());
   
    // ###############################
    //  audio
    //  loading audios part
    // ###############################

    /// option for this music
    SDL_PropertiesID options;
    options = SDL_CreateProperties();
    SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    ENG::Services::Music().LoadMusic("Music/MarineHoloLive.mp3", "Marine", options);
    ENG::Services::Music().SetVolume("Marine", 20.0f);

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
      ENG::Services::Scenes().PeddingScene(ENG::SCENE_MENU);
      ENG::Services::Music().StopMusic("Marine");
    }

    if (pollEvent.IsKeyPress(SDL_SCANCODE_T))
    {
      ENG::Services::SFX().PlaySFX("Shot");
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

    // if (pollEvent.IsKeyDown(SDL_SCANCODE_W))
    // {
    //   pool.Get(PLAYER)->GetTransform().direction.y -= 1.0f;
    // }
    // if (pollEvent.IsKeyDown(SDL_SCANCODE_S))
    // {
    //   pool.Get(PLAYER)->GetTransform().direction.y += 1.0f;
    // }
    // if (pollEvent.IsKeyDown(SDL_SCANCODE_A))
    // {
    //   pool.Get(PLAYER)->GetTransform().direction.x -= 1.0f;
    // }
    // if (pollEvent.IsKeyDown(SDL_SCANCODE_D))
    // {
    //   pool.Get(PLAYER)->GetTransform().direction.x += 1.0f;
    // }

    float wheel = pollEvent.GetMouseWheel();
    if (wheel != 0.0f)
    {
      float zoom = camera->GetZoom() + wheel * 0.1f;
      camera->SetZoom(std::clamp(zoom, 2.0f, 3.0f));
    }
  }

  void DemoScene::Update(float dt)
  {
    for (auto& o : pool.GetAllIDs())
    {
      pool.Get(o)->Update(dt);
    }

  }

  void DemoScene::UpdateFixed(float dt)
  {
    (void)dt;
  }

  void DemoScene::Render(ENG::Batcher& b)
  {
    // auto camRect = camera->GetRectCamera();
    RenderTileMaps(b, LAYER_BACKGROUND, LAYER_PLAYER);      /// Layer back of the player
    for (auto& entry : renderQueue)
    {
      auto* obj = pool.Get(entry.id);

      obj->Draw(b);
      // if (auto* bb = obj->GetComponent<ENG::IBoundingBox>())
      // {
      //   ENG::Rect objRect = { bb->GetPosition().x, bb->GetPosition().y, bb->GetSize().x, bb->GetSize().y};
      //   if (!RectIntersects(camRect, objRect))
      //     continue;
      // }
    }
    RenderTileMaps(b, LAYER_PLAYER, LAYER_MAX);    /// Layer front of the player

    // ENG::Drawer::DrawLine({300.0f, 300.0f}, {500.0f, 300.0f}, ENG::Color::Green, 2.0f);
    ENG::Drawer::DrawCircle({150.0f, 150.0f}, 30.0f, ENG::Color::Green, 64);
    ENG::Drawer::DrawCircleOutLine({150.0f, 150.0f}, 30.0f, ENG::Color::Yellow, 64);
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
} // namespace APP
