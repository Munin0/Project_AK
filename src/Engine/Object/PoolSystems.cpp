// | -------------------------------
#include "PoolSystems.hpp"
// | -------------------------------
#include "Engine/Inputs/Mouse.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Batching/RenderEntry.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Render/RenderSpace.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <cstdint>
#include <vector>
// | -------------------------------

namespace ENG
{
  namespace //// Private Methods fot the PoolSystems archive.
  {
    /// @brief Shared body of both RenderObjects(); @p includeMax draws maxLayer too.
    void DrawRange(ObjectPool& pool, Batcher& b, uint8_t minLayer, uint8_t maxLayer, bool includeMax)
    {
      // World objects use the active camera, Screen objects none (fixed on screen).
      // SetCamera2D() only flushes when the camera really changes, and the queue
      // is grouped by space inside each layer, so this costs a few flushes per frame.
      Camera2D* worldCamera = b.GetCamera2D();
      for (const RenderEntry& entry : pool.Sort())
      {
        if (entry.m_layer < minLayer)
          continue;
        if (entry.m_layer > maxLayer || (entry.m_layer == maxLayer && !includeMax))
          break;   // the queue is sorted by layer

        Object* obj = pool.Get(entry.m_id);
        if (!obj || !obj->IsVisible())
          continue;

        b.SetCamera2D(entry.m_space == RenderSpace::World ? worldCamera : nullptr);
        obj->Draw(b);
      }
      b.SetCamera2D(worldCamera);   // what comes next (tile maps...) is in the world again
    }
  }

  void UpdateObjects(ObjectPool& pool, float dt)
  {
    for (ObjectID id : pool.GetAllIDs())
      pool.Get(id)->Update(dt);
  }

  void DispatchPointer(ObjectPool& pool)
  {
    const Vector2   screenMouse = GetMousePosition();
    const Camera2D* camera      = Render::Get().GetBatcher().GetCamera2D();
    const Vector2   worldMouse  = camera ? camera->ScreenToWorld(screenMouse) : screenMouse;

    // Copy of the order: Sort() returns a cached queue that would be rebuilt
    // if a callback changed the pool while we walk it.
    const std::vector<RenderEntry> queue = pool.Sort();
    for (auto it = queue.rbegin(); it != queue.rend(); ++it)   // top layer first
    {
      Object* obj = pool.Get(it->m_id);
      if (!obj || !obj->IsVisible())
        continue;
      obj->OnPointer(it->m_space == RenderSpace::World ? worldMouse : screenMouse);
    }
  }

  void RenderObjects(ObjectPool& pool, Batcher& b)
  {
    DrawRange(pool, b, 0, 255, true);
  }

  void RenderObjects(ObjectPool& pool, Batcher& b, uint8_t minLayer, uint8_t maxLayer)
  {
    DrawRange(pool, b, minLayer, maxLayer, false);
  }
}
