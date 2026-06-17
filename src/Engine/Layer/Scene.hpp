#pragma once

#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Render/RBatch.hpp"
#include <cstdint>

namespace ENG
{
  class Scene 
  {
    public:
      virtual ~Scene() = default;

      virtual void Init(ENG::ObjectPool& pool){};
      virtual void Destroy(ENG::ObjectPool& pool){};
      virtual bool IsRunning(void)  {return isRunning;}
      virtual bool IsInit()     {return isInit;}
      void SetInit(bool _) {isInit = _;}; 
      virtual void Inputs(float dt,ENG::ObjectPool& pool){(void)dt;};
      virtual void Update(float dt,ENG::ObjectPool& pool){(void)dt;};
      virtual void Render(ENG::Batcher& b, ENG::ObjectPool& pool) = 0;
  
      uint8_t GetSceneID(void) const
      {
        return this->sceneID;
      }
    protected:
      bool isRunning = false;
      bool isInit = false;
      uint8_t sceneID;
  };
}
