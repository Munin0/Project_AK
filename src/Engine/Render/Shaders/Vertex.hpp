// | -------------------------------
#pragma once
// | -------------------------------
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Vertex sent to the GPU by the batch renderer.
   *
   * Each vertex carries its position, color, texture coordinates and the
   * extra data the shader needs to pick a texture and apply an effect.
   * All the members are floats, so the struct is tightly packed. The
   * layout must stay in sync with the vertex attributes configured in
   * OpenGL and with the inputs of the vertex shader.
   */
  struct Vertex
  {
    /// @brief Position of the vertex: { x, y, z }.
    glm::vec3 m_position;
    /// @brief Color of the vertex, as RGBA.
    glm::vec4 m_color;
    /// @brief Texture coordinates (UV) of the vertex.
    glm::vec2 m_texCoord;
    /// @brief Texture slot to sample from. 0.0 selects the white texture.
    float     m_texIndex;
    /// @brief Layer to sample from in the reserved texture array slot.
    ///        -1.0 means that the vertex does not sample from the texture array.
    float     m_arrayLayer;
    /// @brief Effect applied by the shader: 0 = normal, 1 = monochromatic, etc.
    float     m_effectMode;
  };
}
