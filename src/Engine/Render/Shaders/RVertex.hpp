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
    glm::vec3 Position;  // X,Y,Z
    glm::vec4 Color;
    glm::vec2 TexCord;
    float     TexIndex;   // Texture slot (0.0 = white texture)
    float     ArrayLayer; // Layer in the reserved TextureArray slot, -1.0 = not an array sample
    float     EffectMode; // 0 = Normal, 1 = Monocromatic, etc.
  };
}
