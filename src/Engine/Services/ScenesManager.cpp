// | -------------------------------
#include "ScenesManager.hpp"
// | -------------------------------
#include "Engine/Layer/Scene.hpp"
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <cassert>
#include <memory>
#include <string>
#include <utility>
// | -------------------------------

namespace ENG
{
  ScenesManager::~ScenesManager()
  {
    scenesMap.clear();
  }

  void ScenesManager::AddScene(std::unique_ptr<Scene> _)
  {
    auto id =_->GetSceneID();
    if(scenesMap.contains(id))
    {
      LOG_ERROR(" | << Scene already in the Manager");
      return;
    }
    scenesMap.emplace(id,std::move(_));
    if(scenesMap.size() == 1)
      s_current = scenesMap[id].get();
  }

  Scene* ScenesManager::GetCurrent()
  {    
    assert(s_current != nullptr && "ScenesManager: No active scene");
    return s_current;
  }

  void ScenesManager::ChangeScene(void)
  { 
    if(!arePedding)
    {
        LOG_ERROR(" | << ChangeScene called with no pending scene");
        return;
    }
    if(!scenesMap.contains(pedding))
    {
      LOG_ERROR(" | << Scene is not in the Manager: ID[" + std::to_string(pedding)+"]");
      return;
    }
    s_current = scenesMap.at(pedding).get();
    arePedding = false;
  }

  void ScenesManager::PeddingScene(SceneID idPedding)
  {
    if(idPedding == pedding)
    {
      LOG_INFO(" | << Pedding change: Scene current is equals the pedding");
      return;
    }
    pedding = idPedding;
    arePedding = true;
  }

}
