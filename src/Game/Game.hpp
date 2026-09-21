/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Layer/GameLayer.hpp"
/// | ------------------------------------ |
#define PLAYER 1
/// | ------------------------------------ |
#define LAYER_BACKGROUND  0
#define LAYER_WOLRD       10
#define LAYER_PLAYER      20
#define LAYER_MIDDLE      25
#define LAYER_FRONT       30
#define LAYER_FX          40
#define LAYER_UI          50
#define LAYER_MAX         255
/// | ------------------------------------ |

namespace APP
{
  class Game: public ENG::GameLayer
  {
    public:
      //! Explicit constructor, we dont need convertions
      explicit Game();
      //! Defatult destructor #Inheritance
      ~Game()override = default;
      //! Init all stuf for the game. #Inheritance
      void OnInit(void) override;
      //! First inputs to take, #Inheritance
      void OnInputs(float dt ) override;
      //! Destroy all stuf of the game, #Inheritance
      void OnDestroy(void) override;
      //! Validated if the game still running, #Inheritance
      bool IsRunning(void) override;
      //! Update all the Game, #Inheritance.
      void OnUpdate(float dt) override;
      //! UpdateFixed (For physics), #Inheritance.
      void OnUpdateFixed(float dt) override;
      //! Render all stuf of the Game, #Inheritance.
      void OnRender(float dt) override;
    private:
      bool m_gameRunning = false;     //! Boolean, if the game is running.
  };
}
