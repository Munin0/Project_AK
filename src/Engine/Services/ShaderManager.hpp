// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/Shaders/RShader.hpp"
// | -------------------------------
#include <map>
#include <memory>
#include <string>
// | -------------------------------

namespace ENG
{
  class ShaderManager
  {
    public:
      void Clear();
      void Load(const std::string& frag, const std::string& vert, const std::string& idKey);
      Shader* Get(const std::string& idKey);
      void Draw(const std::string& idKey);
    private:
      std::map<std::string, std::unique_ptr<Shader>> m_shaders;
  };
}

