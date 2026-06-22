// | -------------------------------
#pragma once
// | -------------------------------
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
// | -------------------------------

namespace ENG
{
  struct Vertex
  {
    glm::vec3 Position;  // X,Y + Z (Z sorting layers)
    glm::vec4 Color;
    glm::vec2 TexCord;
    float     TexIndex; // Texture slot (0.0 = white texture)
  };
}
