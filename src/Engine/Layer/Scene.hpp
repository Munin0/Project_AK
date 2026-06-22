// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
// | -------------------------------
#include <cstdint>
#include <memory>
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
      bool isRunning = false;
      bool isInit = false;
      uint8_t sceneID;
      std::unique_ptr<Camera2D> camera;
  };
}
