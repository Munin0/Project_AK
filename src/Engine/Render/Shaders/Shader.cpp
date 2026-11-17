// | -------------------------------
#include "Shader.hpp"
// | -------------------------------
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  int Shader::s_countShaders = 0;

  Shader::Shader(const std::string& frag, const std::string& vert)
    : m_vert{vert}, m_frag{frag}
  {
    bool result = this->CompileShader();
    if(result)
      LOG_INFO(" |    >> Shader program created succesfully");
    else
      LOG_INFO(" |    >> Shader program NOT created");
  };

  void Shader::ToString(void) const
  {
    LOG_INFO("Program: " + std::to_string(this->m_program) + " Vertex: " + this->m_vert + " Fragment: " + this->m_frag);
    LOG_INFO("VertexID: " + std::to_string(this->m_vertex) + " FragmentID: " + std::to_string(m_fragment));
  }
}
