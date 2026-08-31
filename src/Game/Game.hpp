/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Layer/GameLayer.hpp"
#include "Engine/Render/Color/RColor.hpp"
/// | ------------------------------------ |
#include <cstdint>
#include <string>
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
  // Maps a Tiled layer name to the engine's draw-order layer (LAYER_* above), so tile map
  // layers are placed in front of/behind other objects just by naming them in Tiled --
  // no per-map C++ wiring needed. Unrecognized names fall back to LAYER_BACKGROUND.
  inline uint8_t TiledLayerToEngineLayer(const std::string& name)
  {
    if (name == "Background") return LAYER_BACKGROUND;
    if (name == "World")      return LAYER_WOLRD;
    if (name == "Middle")     return LAYER_MIDDLE;
    if (name == "Front")      return LAYER_FRONT;
    if (name == "FX")         return LAYER_FX;
    if (name == "UI")         return LAYER_UI;
    return LAYER_BACKGROUND;
  }

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
      void OnUpdateFixed(float dt) override;
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
