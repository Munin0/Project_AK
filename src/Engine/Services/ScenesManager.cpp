// | -------------------------------
#include "ScenesManager.hpp"
// | -------------------------------
#include "Engine/Layer/Scene.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <cassert>
#include <memory>
#include <string>
#include <utility>
// | -------------------------------

namespace ENG
{
  void ScenesManager::AddScene(std::unique_ptr<Scene> _)
  {
    auto id =_->GetSceneID();
    if(m_scenesMap.contains(id))
    {
      LOG_ERROR(" | << Scene already in the Manager");
      return;
    }
    m_scenesMap.emplace(id,std::move(_));
    if(m_scenesMap.size() == 1)
      m_s_current = m_scenesMap[id].get();
  }

  Scene* ScenesManager::GetCurrent()
  {    
    assert(m_s_current != nullptr && "ScenesManager: No active scene");
    return m_s_current;
  }

  void ScenesManager::ChangeScene(void)
  { 
    if(!m_arePedding)
    {
        LOG_ERROR(" | << ChangeScene called with no pending scene");
        return;
    }
    if(!m_scenesMap.contains(m_pedding))
    {
      LOG_ERROR(" | << Scene is not in the Manager: ID[" + std::to_string(m_pedding)+"]");
      return;
    }
    Render::Get().GetBatcher().SetCamera2D(nullptr);
    m_s_current = m_scenesMap.at(m_pedding).get();
    m_arePedding = false;
  }

  void ScenesManager::PeddingScene(SceneID idPedding)
  {
    if(idPedding == m_pedding)
    {
      LOG_INFO(" | << Pedding change: Scene current is equals the pedding");
      return;
    }
    m_pedding = idPedding;
    m_arePedding = true;
  }

}
