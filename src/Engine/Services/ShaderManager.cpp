// | -------------------------------
#include "ShaderManager.hpp"
// | -------------------------------
#include "Engine/Render/Render.hpp"
#include "Engine/Render/Shaders/RShader.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
#include <memory>
#include <string>
#include <utility>
// | -------------------------------

namespace ENG
{
  void ShaderManager::Load(const std::string& frag, const std::string& vert, const std::string& idKey)
  {
    auto& path = Path::Get();
    auto p = Path::Get().ShadersPath;

    auto _fPath = path.ReadFile(p / frag);
    auto _vPath = path.ReadFile(p / vert);

    auto s = std::make_unique<Shader>(_vPath,_fPath);
    shaders[idKey] = std::move(s);
  }

  Shader* ShaderManager::Get(const std::string& idKey)
  {
    auto it = shaders.find(idKey);
    return (it != shaders.end()) ? it->second.get() : nullptr;
  }

  void ShaderManager::Draw(const std::string& idKey)
  {
    auto& s = shaders[idKey];
    auto& b = Render::Get().GetBatcher();

    b.SetMaterial(s.get());
  }
}

