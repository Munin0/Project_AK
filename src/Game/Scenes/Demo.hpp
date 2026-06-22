/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Layer/Scene.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Batching/RenderEntry.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include <vector>
/// | ------------------------------------ |

namespace APP
{
  class DemoScene : public ENG::Scene
  {
    public:
      DemoScene(ENG::SceneID id);

      void Init(void) override;
      void Destroy(void)    override;
      bool IsRunning(void)  override; 
      void Inputs(float dt) override;
      void Update(float dt) override;
      void Render(ENG::Batcher& b) override;
    
    private:
      ENG::ObjectPool pool;
      std::vector<ENG::RenderEntry> renderQueue;
  };
}
