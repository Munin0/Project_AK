/// | ------------------------------------ |
#include "MainMenu.hpp"
/// | ------------------------------------ |
#include "Engine/GUI/Button.hpp"
#include "Engine/GUI/Label.hpp"
#include "Engine/GUI/TextBox.hpp"
#include "Engine/Inputs/Mouse.hpp"
#include "Engine/Inputs/PollEvent.hpp"
#include "Engine/Layer/Scene.hpp"
#include "Engine/Physics/Collision.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Object/PoolSystems.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Color/Color.hpp"
#include "Engine/Render/Layers.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Services/FontManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | ------------------------------------ |
#include "Game/Systems/Functions.hpp"
/// | ------------------------------------ |
#include "SDL3/SDL_mouse.h"
/// | ------------------------------------ |
#include <any>
#include <memory>
#include <string>
#include <utility>
/// | ------------------------------------ |

namespace
{
  const ENG::Vector2     BUTTON_SIZE{220.0f, 60.0f};
  constexpr float        BUTTON_GAP = 20.0f;
}

namespace APP
{
  MainMenu::MainMenu(const std::string& id) : ENG::Scene{}
  {
    m_sceneId = id;
  }

  void MainMenu::Init(void)
  {
    if (m_isInit)
      return;


    const ENG::Vector2 screen = ENG::Render::Get().GetScreenSize();
    const float        cx     = screen.x * 0.5f;
    ENG::Font*         font   = ENG::Services::Fonts().GetFont("CabinItalic");
    if (!font)
    {
      LOG_ERROR(" | << MainMenu: font 'CabinItalic' not loaded");
      return;
    }

    // ###############################
    // Title (Label)
    // ###############################
    const ENG::Vector2 titleSize{600.0f, 120.0f};
    auto* title = static_cast<ENG::Label*>(
      m_pool.Get(m_pool.Add(std::make_unique<ENG::Label>("Title", titleSize))));
    title->SetFont(*font);
    title->SetText("Project AK");
    title->SetFontSize(72);
    title->SetColorText(ENG::Color::Black);
    title->SetBackgroundColor(ENG::Color::White);
    title->SetPosition({cx - titleSize.x * 0.5f, screen.y * 0.15f});
    title->SetTextAlign(ENG::AlignText::TEXT_ALIGN_CENTER);   // after SetPosition: it uses it
    title->SetLayer(LAYER_UI);

    // ###############################
    // Name field (TextBox)
    // ###############################
    const ENG::Vector2 boxSize{400.0f, 60.0f};
    auto* nameBox = static_cast<ENG::TextBox*>(m_pool.Get(m_pool.Add(std::make_unique<ENG::TextBox>("NameBox", "Ingresa nombre", boxSize))));
    nameBox->SetFont(*font);
    nameBox->SetFontSize(28);
    nameBox->SetTextColor(ENG::Color::Black);   // the placeholder is gray by default
    nameBox->SetMaxLength(48);
    nameBox->SetOnSubmit([](const std::string& name)
    {
      LOG_INFO(" | << MainMenu: player name '" + name + "'");
    });
    nameBox->SetPosition({cx - boxSize.x * 0.5f, screen.y * 0.40f});
    nameBox->SetLayer(LAYER_UI);

    // ###############################
    // Buttons
    // ###############################
    const float buttonsY = screen.y * 0.40f + boxSize.y + 2.0f * BUTTON_GAP;
    auto addButton = [&](const std::string& name, const std::string& text, float y, ENG::Button::ButtonCallback callback, std::any data)
    {
      auto* button = static_cast<ENG::Button*>(
        m_pool.Get(m_pool.Add(std::make_unique<ENG::Button>(name, BUTTON_SIZE))));
      button->SetPosition({cx - BUTTON_SIZE.x * 0.5f, y});
      button->SetFont(*font);
      button->SetText(text);
      button->SetTextSize(28);
      button->SetTextColor(ENG::Color::Black);
      button->SetFunction(std::move(callback));
      button->SetData(std::move(data));
      button->SetLayer(LAYER_UI);
    };

    addButton("PlayButton", "Jugar", buttonsY, ChangeSceneButton, std::string("GameScene"));
    addButton("QuitButton", "Salir", buttonsY + BUTTON_SIZE.y + BUTTON_GAP, QuitGameButton, {});

    m_isInit = true;
  }

  void MainMenu::Destroy(void)
  {
    m_pool.Clear();
    m_isInit  = false;
  }

  void MainMenu::Inputs(float dt)
  {
    (void)dt;
    // buttons and text box take their clicks
    ENG::DispatchPointer(m_pool);
  }

  void MainMenu::Update(float dt)
  {
    ENG::UpdateObjects(m_pool, dt);
  }

  void MainMenu::UpdateFixed(float dt)
  {
    (void)dt;
  }

  void MainMenu::Render(ENG::Batcher& b)
  {
    ENG::RenderObjects(m_pool, b);
  }
}
