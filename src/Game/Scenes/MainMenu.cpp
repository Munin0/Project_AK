/// | ------------------------------------ |
#include "MainMenu.hpp" /// | ------------------------------------ |
#include "Engine/Component/Component.hpp"
#include "Engine/GUI/Button.hpp"
#include "Engine/GUI/Label.hpp"
#include "Engine/Layer/Scene.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Physics/Collision.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Text/Text.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Inputs/Mouse.hpp"
#include "Engine/Utils/Path.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Inputs/PollEvent.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Render/Geometry/RGeometry.hpp"
  /// | ------------------------------------ |
#include "Game/Game.hpp"
#include "Game/Systems/Functions.hpp"
  /// | ------------------------------------ |
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_scancode.h"
  /// | ------------------------------------ |
#include <memory>
#include <string>
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
      // ###############################
      /// Screen Size
      auto sizeScreen = ENG::Render::Get().GetScreenSize();
      auto f_CabinItalic = ENG::Services::Fonts().GetFont("CabinItalic");
    
      ENG::Services::CSV().Load("Dialogs/dialog.csv","Dialogos");

      // ###############################
      // Objects
      // Configuration of the Object entities
      // ###############################
      /// Gui Elements
      ENG::ObjectID btStartID = pool.Add(std::make_unique<ENG::Button>("StartGame", ENG::Vector2{100,50} ));
      auto startBT = static_cast<ENG::Button*>(pool.Get(btStartID));

      startBT->SetPosition({100,100});
      startBT->SetData(static_cast<ENG::SceneID>(ENG::SCENE_DEMO));
      startBT->SetFunction(ChangeSceneButton);
      startBT->SetLayer(LAYER_UI);
      startBT->SetFont(*f_CabinItalic);
      startBT->SetText("Start Game");
      startBT->SetTextSize(24);
      startBT->SetTextColor(ENG::Color::Black);;

      // ###############################
      // Objects Text
      // Configuration of the Object Text
      // ###############################
      // ENG::ObjectID textID = pool.Add(std::make_unique<ENG::Text>(ENG::Vector2{100,100}, 120.0f, "TEXTO DE PRUEBA"));
      // auto textObj = static_cast<ENG::Text*>(pool.Get(textID));
      // textObj->AddComponent<ENG::IColor>(ENG::Color::Black);

      // ###############################
      // Objects Label
      // Configuration of the Object Labels
      // ###############################
      // ENG::ObjectID labelID = pool.Add(std::make_unique<ENG::Label>("IniciarPrueba", ENG::Vector2{250,150}));
      // auto labelObj = static_cast<ENG::Label*>(pool.Get(labelID));
      // labelObj->SetFont(*f_CabinItalic);
      // labelObj->SetText("Iniciar Partida");
      // labelObj->SetPosition({sizeScreen.x/2.0f - 250/2.0f,sizeScreen.y/2.0f - 150/2.0f});
      // labelObj->SetFontSize(64);
      // labelObj->SetOffset({5.0f,5.0f});
      // labelObj->SetTextAlign(ENG::AlignText::TEXT_ALIGN_CENTER);
      // labelObj->SetColorText(ENG::Color::Black);
      // labelObj->SetBackgoundColor(ENG::Color::White);

      // ###############################
      // Final configurations
      // Final configurations for the GameLayer
      // ###############################
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
        mousePressed = false;
        isRunning = false;
        ENG::Services::Scenes().PeddingScene(ENG::SCENE_DEMO);
      }

      if(pollEvent.IsMouseButtonPress(SDL_BUTTON_LEFT))
      {
        mousePressed = true;
      }

      if(pollEvent.IsKeyPress(SDL_SCANCODE_T))
      {
        auto text = ENG::Services::CSV().GetText("eng","DIALOG_2", "Dialogos");
        LOG_INFO(" | << Dialog: " + text);
      }

      if(pollEvent.IsKeyPress(SDL_SCANCODE_Y))
      {
        auto text = ENG::Services::CSV().GetRangeText("esp","DIALOG_2", "DIALOG_4", "Dialogos");
        for(auto& t : text)
          LOG_INFO(" | << TEXT: " + t);
      }
    }

    void MainMenu::Update(float dt)
    {
      for (auto& o : pool.GetAllIDs())
      {
        pool.Get(o)->Update(dt);

        if(auto b = dynamic_cast<ENG::Button*>(pool.Get(o)))
        {
          if(ENG::CollisionPointRect(ENG::GetMousePosition(), b->GetRect()) && mousePressed)
          {
            b->Action(b->GetData());
            isRunning = false;
            mousePressed = false;
          }
        }
      }
      
      mousePressed = false;
    }

    void MainMenu::UpdateFixed(float dt)
    {
      (void)dt;
    }

    void MainMenu::Render(ENG::Batcher& b)
    {
      for(auto& entry : renderQueue)
      {
        pool.Get(entry.id)->Draw(b);
      }
    }

  }
