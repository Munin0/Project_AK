// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/Shaders/RShader.hpp"
#include "Engine/Services/KeyHash.hpp"
// | -------------------------------
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Stores the shaders used by the engine.
   *
   * ShaderManager creates and owns every Shader and lets the rest of the
   * engine retrieve or use them by a string key, so each shader program is
   * compiled once and shared.
   *
   * Usage:
   * @code
   * shaders.Load("basic.frag", "basic.vert", "basic");
   * shaders.Draw("basic");
   * @endcode
   *
   * @note An OpenGL context must be current when a shader is loaded,
   *       because it is compiled at that moment.
   */
  class ShaderManager
  {
    public:
      /**
       * @brief Destroys every stored shader.
       *
       * Any pointer previously returned by Get() becomes invalid after this
       * call.
       */
      void Clear();
      /**
       * @brief Loads a shader and stores it under a key.
       *
       * Note that the fragment shader comes first and the vertex shader
       * second, the opposite order of the Shader constructor.
       *
       * @param[in] frag  Fragment shader file to load.
       * @param[in] vert  Vertex shader file to load.
       * @param[in] idKey Key used to refer to this shader in the other
       *                  functions of the manager.
       */
      void Load(const std::string& frag, const std::string& vert, const std::string& idKey);
      /**
       * @brief Returns a stored shader by key.
       *
       * @param[in] idKey Key the shader was loaded with.
       * @return Pointer to the shader, or nullptr if no shader was loaded
       *         with that key. The shader is owned by the manager and the
       *         pointer becomes invalid after Clear().
       */
      Shader* Get(const std::string& idKey);
      /**
       * @brief Selects a shader to draw with.
       *
       * @param[in] idKey Key the shader was loaded with.
       */
      void Draw(const std::string& idKey);
    private:
      /// @brief Stored shaders, indexed by key.
      std::unordered_map<std::string, std::unique_ptr<Shader>, StringHash, std::equal_to<>> m_shaders;
  };
}
