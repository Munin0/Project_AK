// | -------------------------------
#include "ShaderManager.hpp"
// | -------------------------------
#include "Engine/Render/Render.hpp"
#include "Engine/Render/Shaders/RShader.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
#include <memory>
#include <string>
#include <utility>
// | -------------------------------

namespace ENG
{
  void ShaderManager::Clear()
  {
    m_shaders.clear();
  }

  void ShaderManager::Load(const std::string& frag, const std::string& vert, const std::string& idKey)
  {
    if(m_shaders.contains(idKey))
    {
      LOG_ERROR(" | << ERROR: Shader exits " + idKey);
      return;
    }
    auto& path = Path::Get();
    auto p = Path::Get().ShadersPath;

    auto _fPath = path.ReadFile(p / frag);
    auto _vPath = path.ReadFile(p / vert);

    auto s = std::make_unique<Shader>(_vPath,_fPath);
    m_shaders[idKey] = std::move(s);
  }

  Shader* ShaderManager::Get(const std::string& idKey)
  {
    auto it = m_shaders.find(idKey);
    return (it != m_shaders.end()) ? it->second.get() : nullptr;
  }

  void ShaderManager::Draw(const std::string& idKey)
  {
    auto& s = m_shaders[idKey];
    auto& b = Render::Get().GetBatcher();

    b.SetMaterial(s.get());
  }
}

