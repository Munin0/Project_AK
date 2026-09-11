/// | ------------------------------------ |
#include "Plantilla.hpp"
/// | ------------------------------------ |
#include "Game/Game.hpp"
/// | ------------------------------------ |
#include "Engine/Layer/Scene.hpp"
#include "Engine/Inputs/PollEvent.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
/// | ------------------------------------ |

namespace APP
{
  Plantilla::Plantilla(ENG::SceneID id)
    : ENG::Scene{}
  {
    sceneID = id;
  }

  void Plantilla::Init()
  {
    if (isInit)
      return;

    /// Final configurations
    isRunning = true;
    this->renderQueue = pool.Sort();
  }

  void Plantilla::Inputs(float dt)
  {
    auto& p = ENG::PollEvent::Get();
  }

  void Plantilla::Update(float dt)
  {
    for (auto& o : pool.GetAllIDs())
    {
      pool.Get(o)->Update(dt);
    }

  }

  void Plantilla::UpdateFixed(float dt)
  {
    (void)dt;
  }

  void Plantilla::Render(ENG::Batcher& b)
  {
    for (auto& entry : renderQueue)
    {
      pool.Get(entry.id)->Draw(b);
    }
  }

  void Plantilla::Destroy()
  {
    pool.Clear();
    renderQueue.clear();
    isInit = false;
    isRunning = false;
    tileMaps.clear();
  }

  bool Plantilla::IsRunning()
  {
    return isRunning;
  }
}
