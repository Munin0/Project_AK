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
    SCENE_LEVEL_2 = 3,
    SCENE_DEMO = 100,
  };

  class ScenesManager
  {
    public:
      ScenesManager() = default;
      ~ScenesManager() = default;
      
      void AddScene(std::unique_ptr<Scene> _);
      Scene* GetCurrent();
      void ChangeScene(void);
      void PeddingScene(SceneID idPedding);
      void DestroyScene(SceneID id);
      void Clear(){m_scenesMap.clear();}
    private:
      Scene* m_s_current = nullptr;
      SceneID m_pedding = 1;
      bool m_arePedding = false;
      std::map<SceneID, std::unique_ptr<Scene>> m_scenesMap;
  };

}
