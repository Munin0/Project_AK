/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Layer/GameLayer.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Render/RColor.hpp"
#include "Engine/Render/RenderEntry.hpp"
#include <vector>
/// | ------------------------------------ |
#define PLAYER 1
/// | ------------------------------------ |
#define LAYER_BACKGROUND  0
#define LAYER_WOLRD       10
#define LAYER_PLAYER      20
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
      bool IsAppEnd;
      ENG::ObjectPool pool;
      std::vector<ENG::RenderEntry> renderQueue;

    protected:
      ENG::Color bgColor;
      float changer = 0.0;
  };
}
