// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Map/TileMap/TileMap.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
// | -------------------------------
#include <cstdint>
#include <memory>
#include <vector>
// | -------------------------------

namespace ENG
{
  class Scene
  {
    public:
      virtual ~Scene() = default;

      virtual void Init(void){};
      virtual void Destroy(void){};
      virtual bool IsRunning(void)  {return isRunning;}
      virtual void Inputs(float dt){(void)dt;};
      virtual void Update(float dt){(void)dt;};
      virtual void Render(ENG::Batcher& b) = 0;

      uint8_t GetSceneID(void) const
      {
        return this->sceneID;
      }
    protected:
      // Keeps tileMaps sorted by TileMap::GetLayer() ascending.
      void AddTileMap(TileMap map);
      // Draws tileMaps whose layer is in [minLayer, maxLayer) -- lets a scene
      // interleave tilemap draws with its ObjectPool render queue by layer group.
      void RenderTileMaps(Batcher& b, uint8_t minLayer, uint8_t maxLayer) const;

      bool isRunning = false;
      bool isInit = false;
      uint8_t sceneID;
      std::unique_ptr<Camera2D> camera;
      std::vector<TileMap> tileMaps;
  };
}
