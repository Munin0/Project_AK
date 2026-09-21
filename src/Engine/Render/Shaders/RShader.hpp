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
  /**
   * @brief OpenGL shader program built from a vertex and a fragment shader.
   *
   * Shader compiles the given GLSL sources, links them into an OpenGL
   * program and exposes the program handle so it can be used for rendering.
   * It also keeps the location of the view-projection uniform.
   *
   * @note An OpenGL context must be current when a Shader is created,
   *       because compilation and linking are done in the constructor.
   */
  class Shader
  {
    public:
      /**
       * @brief Compiles and links a shader program.
       *
       * Both parameters are GLSL source code, not file paths. An empty
       * string means that the corresponding stage is skipped.
       *
       * @param[in] vert GLSL source code of the vertex shader.
       * @param[in] frag GLSL source code of the fragment shader.
       */
      Shader(const std::string& vert, const std::string& frag);
      /**
       * @brief Default destructor.
       *
       * @note It does not delete the OpenGL program (glDeleteProgram).
       */
      ~Shader() = default;
      /**
       * @brief Outputs a textual description of the shader.
       */
      void ToString(void) const;
      /**
       * @brief Returns the OpenGL program handle.
       *
       * @return The handle of the linked program, to be used with
       *         glUseProgram().
       */
      GLuint GetProgram() const { return program; }
      /**
       * @brief Returns the location of the view-projection uniform.
       *
       * @return The uniform location. In OpenGL, -1 means that the uniform
       *         was not found in the program.
       */
      GLint GetViewProjectionLoc() const {return m_locViewProjection;}
    private:
      /**
       * @brief Compiles the vertex and fragment shaders and links the program.
       *
       * Private because it is only meant to be called from the constructor.
       *
       * For each non-empty source it creates, compiles and attaches the
       * shader to the program, then links the program. Every step is
       * reported through the log. The intermediate shader objects are
       * deleted once the program is linked, and the internal shader counter
       * is incremented on success.
       *
       * @return true if the program was compiled and linked successfully,
       *         false otherwise.
       *
       * @warning It is declared as returning GLuint, but it only returns
       *          true (1) or false (0).
       */
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

          // Link to Program
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

          // Link to program
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
      /// @brief Number of shader programs compiled and linked successfully so far. Shared by all instances.
      static int countShaders;
      /// @brief OpenGL program handle.
      GLuint program;
      /// @brief Location of the view-projection uniform in the program.
      GLint m_locViewProjection;
      /// @brief GLSL source code of the vertex shader.
      std::string _vert;
      /// @brief GLSL source code of the fragment shader.
      std::string _frag;
      /// @brief Temporary OpenGL handles of the vertex and fragment shaders (vertex, fragment); only used while compiling.
      GLuint vertex, fragment;
  };
}
