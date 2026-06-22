// | -------------------------------
#pragma once
// | -------------------------------

namespace ENG
{
  class GameLayer
  {
    public:
      virtual ~GameLayer() = default;

      virtual void OnInit(void){};
      virtual void OnDestroy(void){};
      virtual bool IsRunning(void){return !gl_Done;};
      virtual void OnInputs(float dt){(void)dt;};
      virtual void OnUpdate(float dt){(void)dt;};
      virtual void OnRender(float dt){(void)dt;};

    protected:
      bool gl_Done;
  };
}
