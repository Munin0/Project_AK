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

  bool ScenesManager::DestroyScene(const std::string& id)
  {
    auto it = m_scenesMap.find(id);
    if(it == m_scenesMap.end())
    {
      LOG_ERROR(" | << Can't find the scene to destroy > DestroyScene: [ " + id + " ]");
      return false;
    }

    if (it->second.get() == m_s_current)
    {
      LOG_ERROR(" | << DestroyScene: can't destroy the current scene [ " + id + " ]");
      return false;
    }

    if (m_hasPeding && m_peding == id)
    {
      m_peding.clear();
      m_hasPeding = false;
    }

    it->second->Destroy();
    m_scenesMap.erase(it);
    return true;
  }

  Scene* ScenesManager::GetCurrent()
  {    
    assert(m_s_current != nullptr && "ScenesManager: No active scene");
    return m_s_current;
  }

  bool ScenesManager::ChangeScene(void)
  { 
    if(!m_hasPeding)
    {
        LOG_ERROR(" | << ChangeScene called with no pending scene");
        return false;
    }

    auto it = m_scenesMap.find(m_peding);
    if(it == m_scenesMap.end())
    {
      LOG_ERROR(" | << Scene is not in the Manager: ID[" + m_peding +"]");
      m_peding.clear();
      m_hasPeding = false;
      return false;
    }

    Render::Get().GetBatcher().SetCamera2D(nullptr);
    m_s_current = it->second.get();
    m_peding.clear();
    m_hasPeding = false;
    return true;
  }

  const std::string& ScenesManager::GetPedingScene(void)
  {
    return this->m_peding;
  }

  void ScenesManager::SetPedingScene(const std::string& id)
  {
    if(id == m_peding)
    {
      LOG_INFO(" | << SetPedingScene: [ "+id+" ] equal to [ " + m_peding + " ].");
      return;
    }
    m_peding.clear();
    m_peding = id;
    m_hasPeding = true;
  }

  bool ScenesManager::GetCheckScene(const std::string& id)
  {
    return m_scenesMap.contains(id);
  }

  bool ScenesManager::HasPedingScene(void) const
  {
    return m_hasPeding; 
  }
}
