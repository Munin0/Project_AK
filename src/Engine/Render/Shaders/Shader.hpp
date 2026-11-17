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
       * @param[in] frag GLSL source code of the fragment shader.
       * @param[in] vert GLSL source code of the vertex shader.
       */
      Shader(const std::string& frag, const std::string& vert);
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
      GLuint GetProgram() const { return m_program; }
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
        LOG_INFO(" | -- SHADER COMPILATION [" + std::to_string(this->s_countShaders) + "]");
        GLint success;
        const char* _VertCompile = m_vert.c_str();
        const char* _FragCompile = m_frag.c_str();
        char infoLog[256];
        m_program = glCreateProgram();

        if(!m_vert.empty())
        {
          m_vertex = glCreateShader(GL_VERTEX_SHADER);
          glShaderSource(m_vertex, 1, &_VertCompile, NULL);
          glCompileShader(m_vertex);

          // CheckErrors
          glGetShaderiv(m_vertex, GL_COMPILE_STATUS, &success);
          if(!success)
          {
            glGetShaderInfoLog(m_vertex, 256, NULL, infoLog);
            LOG_INFO(" |    >> Error with -Vertex- Shader: " + static_cast<std::string>(infoLog));
            glDeleteShader(m_vertex);
            glDeleteProgram(m_program);
            return false;
          }
          LOG_INFO(" |    >> Vertex Shader compile successfully");

          // Link to Program
          glAttachShader(m_program,m_vertex);
        }
        else {
          LOG_INFO(" |    >> Vertex is Empty");
        }

        if(!m_frag.empty())
        {
          m_fragment = glCreateShader(GL_FRAGMENT_SHADER);
          glShaderSource(m_fragment, 1, &_FragCompile, NULL);
          glCompileShader(m_fragment);

          // CheckErrors
          glGetShaderiv(m_fragment,GL_COMPILE_STATUS, &success);
          if(!success)
          {
            glGetShaderInfoLog(m_fragment, 256, NULL, infoLog);
            LOG_INFO(" |    >> Error with -Fragment- Shader: " + static_cast<std::string>(infoLog));
            glDeleteShader(m_fragment);
            if(!m_vert.empty())
              glDeleteShader(m_vertex);

            glDeleteProgram(m_program);
            return false;
          }
          LOG_INFO(" |    >> Fragment Shader compile successfully");

          // Link to program
          glAttachShader(m_program, m_fragment);
        }
        else {
          LOG_INFO(" |    >> Fragment is Empty");
        }

        glLinkProgram(m_program);
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);

        if(!success)
        {
          glGetProgramInfoLog(m_program, 256, NULL, infoLog);
          LOG_INFO(" |    >> Error with -Program- link: " + static_cast<std::string>(infoLog));
          return false;
        }
        LOG_INFO(" |    >> Program linked successfully");

        if(!m_vert.empty())
          glDeleteShader(m_vertex);
        if(!m_frag.empty())
          glDeleteShader(m_fragment);

        s_countShaders ++;
        return true;
      }
      /// @brief Number of shader programs compiled and linked successfully so far. Shared by all instances.
      static int s_countShaders;
      /// @brief OpenGL program handle.
      GLuint          m_program;
      /// @brief Location of the view-projection uniform in the program.
      GLint           m_locViewProjection;
      /// @brief GLSL source code of the vertex shader.
      std::string     m_vert;
      /// @brief GLSL source code of the fragment shader.
      std::string     m_frag;
      /// @brief Temporary OpenGL handles of the vertex and fragment shaders (vertex, fragment); only used while compiling.
      GLuint          m_vertex, m_fragment;
  };
}
