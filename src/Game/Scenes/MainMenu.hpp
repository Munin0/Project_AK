/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Layer/Scene.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Render/RBatch.hpp"
#include "Engine/Services/ScenesManager.hpp"
/// | ------------------------------------ |

namespace APP
{
  class MainMenu : public ENG::Scene
  {
    public:
      MainMenu(ENG::SceneID id);

      void Init(ENG::ObjectPool& pool) override;
      void Destroy(ENG::ObjectPool& pool)    override;
      bool IsRunning(void)  override; 
      void Inputs(float dt,ENG::ObjectPool& pool) override;
      void Update(float dt,ENG::ObjectPool& pool) override;
      void Render(ENG::Batcher& b,ENG::ObjectPool& pool) override;

      bool IsInit() override {return isInit;}; 
      void SetInit(bool _) {isInit = _;}; 
  };
}


