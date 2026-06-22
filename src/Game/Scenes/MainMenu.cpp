/// | ------------------------------------ |
#include "MainMenu.hpp"
/// | ------------------------------------ |
#include "Game/Game.hpp"
/// | ------------------------------------ |
#include "Engine/Layer/Scene.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/PollEvent/PollEvent.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Geometry/RGeometry.hpp"
/// | ------------------------------------ |
#include "SDL3/SDL_scancode.h"
/// | ------------------------------------ |
#include <utility>
#include <memory>
/// | ------------------------------------ |

namespace APP
{
  MainMenu::MainMenu(ENG::SceneID id)
    : ENG::Scene{}
  {
    sceneID = id;
  }

  void MainMenu::Init(void)
  {
    if(isInit)
    {
      LOG_INFO(" | << SCENE MAINMENU ALREADY INIT");
      return;
    }

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

    
    /// Always last
    isInit = true;
    isRunning = true;
    this->renderQueue = pool.Sort();
  }

  void MainMenu::Destroy(void)
  {
    pool.Clear();
    renderQueue.clear();
    isInit = false;
    isRunning = false;
  }

  bool MainMenu::IsRunning()
  {
    return isRunning;
  }

  void MainMenu::Inputs(float dt)
  {
    auto& pollEvent = ENG::PollEvent::Get();
    
    if(pollEvent.IsKeyPress(SDL_SCANCODE_P))
    {
      isRunning = false;
      // Cambio
      ENG::Services::Scenes().PeddingScene(ENG::SCENE_DEMO);
    }
  }

  void MainMenu::Update(float dt)
  { 
    this->renderQueue = pool.Sort();
  }

  void MainMenu::Render(ENG::Batcher& b)
  { 
    for(auto& entry : renderQueue)
    {
      pool.Get(entry.id)->Draw(b);
    }
  }

}
