// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Layer/Scene.hpp"
// | -------------------------------
#include <map>
#include <cstdint>
#include <memory>
// | -------------------------------

namespace ENG
{
  using SceneID = uint8_t;

  enum : SceneID
  {
    SCENE_MENU = 0,
    SCENE_DEMO = 100,
  };

  class ScenesManager
  {
    public:
      ScenesManager() = default;
      ~ScenesManager();
      
      void AddScene(std::unique_ptr<Scene> _);
      Scene* GetCurrent();
      void ChangeScene(void);
      void PeddingScene(SceneID idPedding);
      void DestroyScene(SceneID id);
    private:
      Scene* s_current = nullptr;
      SceneID pedding = 1;
      bool arePedding = false;
      std::map<SceneID, std::unique_ptr<Scene>> scenesMap;
  };

}
