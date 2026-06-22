/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Layer/GameLayer.hpp"
#include "Engine/Render/Color/RColor.hpp"
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
/// | ------------------------------------ |

namespace APP
{
  class Game: public ENG::GameLayer
  {
    public:
      explicit Game();
      ~Game()override{};
      void OnInit(void) override;
      void OnInputs(float dt ) override;
      void OnDestroy(void) override;
      bool IsRunning(void) override;
      void OnUpdate(float dt) override;
      void OnRender(float dt) override;
    private:
      bool IsAppEnd = false;
      bool isGameInit = false;

      /// Debug FLAGS
      bool debugDraw = false;
    protected:
      ENG::Color bgColor;
      float changer = 0.0;
  };
}
