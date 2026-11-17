/// | ------------------------------------ |
#include "GameScene.hpp"
/// | ------------------------------------ |
#include "Engine/Component/Component.hpp"
#include "Engine/GUI/Label.hpp"
#include "Engine/Inputs/PollEvent.hpp"
#include "Engine/Layer/Scene.hpp"
#include "Engine/Map/CollisionGrid.hpp"
#include "Engine/Map/CollisionGridDebug.hpp"
#include "Engine/Map/MapData.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Object/PoolSystems.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Batching/Drawer.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
#include "Engine/Render/Color/Color.hpp"
#include "Engine/Render/Layers.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Render/RenderSpace.hpp"
#include "Engine/Services/AssetsManager.hpp"
#include "Engine/Services/FontManager.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | ------------------------------------ |
#include "Game/Map/LayerRules.hpp"
#include "Game/Map/Spawn.hpp"
#include "Game/Systems/Systems.hpp"
/// | ------------------------------------ |
#include "Importers/Tiled/TiledLoader.hpp"
/// | ------------------------------------ |
#include "SDL3/SDL_scancode.h"
/// | ------------------------------------ |
#include <memory>
#include <string>
#include <utility>
#include <vector>
/// | ------------------------------------ |

namespace
{
  // Level
  constexpr const char* MAP_PATH   = "Map/Scenes/exampleIsometrico.json";
  constexpr const char* ATLAS_PATH = "Atlas/Isometrico/CaveMap.json";
  constexpr const char* ATLAS_KEY  = "I_CaveMap";
  constexpr float       MAP_SCALE  = 1.0f;

  // Player
  constexpr float PLAYER_SIZE  = 16.0f;
  constexpr float PLAYER_SPEED = 120.0f;
  constexpr float CAMERA_ZOOM  = 1.5f;

  // Dialog
  constexpr const char* DIALOG_CSV      = "Dialogs";   // key used in Game::OnInit
  constexpr const char* DIALOG_LANGUAGE = "esp";
  constexpr float       DIALOG_HEIGHT   = 140.0f;
  constexpr float       DIALOG_MARGIN   = 40.0f;
  constexpr float       DIALOG_FONT     = 32.0f;
}

namespace APP
{
  GameScene::GameScene(const std::string& id) : ENG::Scene{}
  {
    m_sceneId = id;
  }

  void GameScene::Init(void)
  {
    if (m_isInit)
      return;

    // ###############################
    // Player: a plain box. Map collisions use the bottom part of its box.
    // ###############################
    m_playerId = m_pool.Add(std::make_unique<ENG::Object>("Player"));
    ENG::Object* player = m_pool.Get(m_playerId);
    player->AddComponent<ENG::IBoundingBox>(ENG::Vector2{PLAYER_SIZE, PLAYER_SIZE});
    player->SetLayer(LAYER_PLAYER);

    // ###############################
    // Camera: follows the player (GameScene::Update)
    // ###############################
    const ENG::Vector2 screen = ENG::Render::Get().GetScreenSize();
    m_camera = std::make_unique<ENG::Camera2D>(screen.x, screen.y);
    m_camera->SetZoom(CAMERA_ZOOM);
    ENG::Render::Get().GetBatcher().SetCamera2D(m_camera.get());

    // ###############################
    // Map
    // ###############################
    if (LoadLevel())
      SpawnPlayer();

    // ###############################
    // Dialog
    // ###############################
    if (const ENG::ObjectID boxId = CreateDialogBox(); boxId != ENG::NULL_ID)
      m_dialog.SetBox(m_pool, boxId);

    // LOG_INFO(" | << GameScene: WASD move | G collision | Backspace menu");
    m_isInit = true;
  }

  void GameScene::Destroy(void)
  {
    m_dialog.Stop();
    m_dialog.ClearBox();   // its Label goes away with m_pool.Clear()

    m_pool.Clear();
    m_map.Clear();
    m_objects.clear();
    m_playerId = ENG::NULL_ID;
    m_moveDir  = {0.0f, 0.0f};
    m_isInit   = false;
  }

  void GameScene::Inputs(float dt)
  {
    (void)dt;
    ENG::DispatchPointer(m_pool);   // UI first: a click it takes is not seen below
    auto& in = ENG::PollEvent::Get();

    // Dialog: E starts the test conversation, or advances the open one.
    if (in.IsKeyPress(SDL_SCANCODE_E))
    {
      if (m_dialog.IsActive())
        m_dialog.Advance();
      else
        m_dialog.Start(DIALOG_CSV, DIALOG_LANGUAGE, "DIALOG_1", "DIALOG_4");
    }

    if (in.IsKeyPress(SDL_SCANCODE_BACKSPACE))
      ENG::Services::Scenes().SetPedingScene("MainMenu");

    if (m_dialog.IsActive())
      return;   // no movement while talking

    if (in.IsKeyDown(SDL_SCANCODE_W) || in.IsKeyDown(SDL_SCANCODE_UP))    m_moveDir.y -= 1.0f;
    if (in.IsKeyDown(SDL_SCANCODE_S) || in.IsKeyDown(SDL_SCANCODE_DOWN))  m_moveDir.y += 1.0f;
    if (in.IsKeyDown(SDL_SCANCODE_A) || in.IsKeyDown(SDL_SCANCODE_LEFT))  m_moveDir.x -= 1.0f;
    if (in.IsKeyDown(SDL_SCANCODE_D) || in.IsKeyDown(SDL_SCANCODE_RIGHT)) m_moveDir.x += 1.0f;

    if (in.IsKeyPress(SDL_SCANCODE_G))
      m_showCollision = !m_showCollision;
  }

  void GameScene::Update(float dt)
  {
    // If the player was removed, Get() returns nullptr: the camera stays where it was.
    if (ENG::Object* player = m_pool.Get(m_playerId))
    {
      System_MoveOnGrid(*player, m_moveDir, PLAYER_SPEED, m_map.GetCollision(), dt);
      if (m_camera)
        m_camera->Follow(player->GetPosition(), dt);
    }
    m_moveDir = {0.0f, 0.0f};

    ENG::UpdateObjects(m_pool, dt);   // includes the dialog box (letter-by-letter effect)
  }

  void GameScene::UpdateFixed(float dt)
  {
    (void)dt;
  }

  void GameScene::Render(ENG::Batcher& b)
  {
    m_map.Render(b, LAYER_GROUND, LAYER_PLAYER);   // behind the player

    ENG::RenderObjects(m_pool, b, LAYER_GROUND, LAYER_UI_BG);     // world objects
    if (const ENG::Object* player = m_pool.Get(m_playerId))
      ENG::Drawer::DrawRectangle(player->GetPosition(), {PLAYER_SIZE, PLAYER_SIZE}, ENG::Color(0.2f, 0.6f, 1.0f, 1.0f));

    m_map.Render(b, LAYER_PLAYER, LAYER_MAX);      // in front of the player

    if (m_showCollision)
      ENG::DrawCollisionGrid(m_map.GetCollision());

    // UI last, so foreground tiles never cover it. Screen-space objects ignore
    // the camera; hidden ones (the dialog box when closed) are skipped.
    ENG::RenderObjects(m_pool, b, LAYER_UI_BG, LAYER_MAX);
  }

  // | ====================================================== |
  // | Own methods of the scene                               |
  // | ====================================================== |

  bool GameScene::LoadLevel()
  {
    if (!ENG::Services::Assets().LoadAtlas(ATLAS_PATH, ATLAS_KEY))
    {
      LOG_ERROR(std::string(" | << GameScene: cannot load atlas ") + ATLAS_PATH);
      return false;
    }

    auto map = IMP::TiledLoader::Load(MAP_PATH);
    if (!map)
    {
      LOG_ERROR(std::string(" | << GameScene: cannot load map ") + MAP_PATH);
      return false;
    }

    // Drawable layers + collision grid, resolved with the game's layer table
    if (!m_map.Load(*map, {ATLAS_KEY}, GameLayerRules(), MAP_SCALE))
      return false;
    m_objects = std::move(map->m_objects);
    return true;
  }

  void GameScene::SpawnPlayer()
  {
    ENG::Object* player = m_pool.Get(m_playerId);
    if (!player)
      return;

    const auto feet = FindSpawnPoint(m_map.GetCollision(), m_objects);
    if (!feet)
    {
      LOG_ERROR(" | << GameScene: no free cell to spawn the player");
      return;
    }

    // The collision footprint is the bottom of the box: put it on the spawn point.
    const ENG::Vector2 pos = {feet->x - PLAYER_SIZE * 0.5f, feet->y - PLAYER_SIZE + 1.0f};
    player->SetPosition(pos.x, pos.y);
    player->SetPosition(pos.x, pos.y);  // twice: previous position = spawn too
    m_camera->SetPosition(pos);
  }

  ENG::ObjectID GameScene::CreateDialogBox()
  {
    ENG::Font* font = ENG::Services::Fonts().GetFont("CabinItalic");
    if (!font)
    {
      LOG_ERROR(" | << GameScene: font 'CabinItalic' not loaded, no dialog box");
      return ENG::NULL_ID;
    }

    const ENG::Vector2 screen = ENG::Render::Get().GetScreenSize();
    const ENG::Vector2 size{screen.x - 2.0f * DIALOG_MARGIN, DIALOG_HEIGHT};

    const ENG::ObjectID id = m_pool.Add(std::make_unique<ENG::Label>("DialogBox", size));
    auto* box = static_cast<ENG::Label*>(m_pool.Get(id));
    box->SetFont(*font);
    box->SetFontSize(DIALOG_FONT);
    box->SetColorText(ENG::Color::White);
    box->SetBackgroundColor(ENG::Color(0.05f, 0.05f, 0.1f, 0.85f));
    box->SetOffset({24.0f, 24.0f});
    box->SetPosition({DIALOG_MARGIN, screen.y - DIALOG_HEIGHT - DIALOG_MARGIN});
    box->SetTextAlign(ENG::AlignText::TEXT_ALIGN_UPL);   // after SetPosition / SetOffset
    box->SetLayer(LAYER_UI);
    box->SetRenderSpace(ENG::RenderSpace::Screen);       // fixed on screen
    box->SetVisible(false);                              // DialogRunner shows it
    return id;
  }
}
