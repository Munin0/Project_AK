// | -------------------------------
#pragma once
// | -------------------------------
#include <glad/glad.h>
// | -------------------------------
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <cstddef>
#include <string>
// | -------------------------------

namespace ENG
{
  class Shader
  {
    public:
      Shader() = default;
      Shader(const std::string& vert, const std::string& frag);
      ~Shader() = default;

      void ToString(void) const;
      GLuint GetProgram() const { return program; }
    private:
      /// Why private: Because is inside of the constructor
      GLuint CompileShader(void)
      {
        LOG_INFO(" | -- SHADER COMPILATION [" + std::to_string(this->countShaders) + "]");
        GLint success;
        const char* _VertCompile = _vert.c_str();
        const char* _FragCompile = _frag.c_str();
        char infoLog[256];
        program = glCreateProgram();

        if(!_vert.empty())
        {
          vertex = glCreateShader(GL_VERTEX_SHADER);
          glShaderSource(vertex, 1, &_VertCompile, NULL);
          glCompileShader(vertex);

          // CheckErrors
          glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
          if(!success)
          {
            glGetShaderInfoLog(vertex, 256, NULL, infoLog);
            LOG_INFO(" |    >> Error with -Vertex- Shader: " + static_cast<std::string>(infoLog));
            glDeleteShader(vertex);
            glDeleteProgram(program);
            return false;
          }
            LOG_INFO(" |    >> Vertex Shader compile successfully");

          /// Link to Program
          glAttachShader(program,vertex);
        }
        else {
          LOG_INFO(" |    >> Vertex is Empty");
        }

        if(!_frag.empty())
        {
          fragment = glCreateShader(GL_FRAGMENT_SHADER);
          glShaderSource(fragment, 1, &_FragCompile, NULL);
          glCompileShader(fragment);

          // CheckErrors
          glGetShaderiv(fragment,GL_COMPILE_STATUS, &success);
          if(!success)
          {
            glGetShaderInfoLog(fragment, 256, NULL, infoLog);
            LOG_INFO(" |    >> Error with -Fragment- Shader: " + static_cast<std::string>(infoLog));
            glDeleteShader(fragment);
            if(!_vert.empty())
              glDeleteShader(vertex);

            glDeleteProgram(program);
            return false;
          }
          LOG_INFO(" |    >> Fragment Shader compile successfully");

          /// Link to program
          glAttachShader(program, fragment);
        } 
        else {
          LOG_INFO(" |    >> Fragment is Empty");
        }

        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if(!success)
        {
          glGetProgramInfoLog(program, 256, NULL, infoLog);
          LOG_INFO(" |    >> Error with -Program- link: " + static_cast<std::string>(infoLog));
          return false;
        }
        LOG_INFO(" |    >> Program linked successfully");
          
        if(!_vert.empty())
          glDeleteShader(vertex);
        if(!_frag.empty())
          glDeleteShader(fragment);

        countShaders ++;
        return true;
      }
      
      static int countShaders;
      GLuint program;
      std::string _vert;
      std::string _frag;
      GLuint vertex, fragment;
  };
}
