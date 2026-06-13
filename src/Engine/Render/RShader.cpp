// | -------------------------------
#include "RShader.hpp"
// | -------------------------------
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  int Shader::countShaders = 0;

  Shader::Shader(const std::string& vert, const std::string& frag)
    : _vert{vert}, _frag{frag}
  {
    bool result = this->CompileShader();
    if(result)
      LOG_INFO(" |    >> Shader program created succesfully");
    else
      LOG_INFO(" |    >> Shader program NOT created");
  };

  void Shader::ToString(void) const
  {
    LOG_INFO("Program: " + std::to_string(this->program) + " Vertex: " + this->_vert + " Fragment: " + this->_frag);
    LOG_INFO("VertexID: " + std::to_string(this->vertex) + " FragmentID: " + std::to_string(fragment));
  }
}
