// | -------------------------------
#pragma once
// | -------------------------------
#include "RImage.hpp"
#include "RVertex.hpp"
#include "RShader.hpp"
// | -------------------------------
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
// | -------------------------------
#include <array>
#include <cstdint>
#include <cstdint>
#include <memory>
#include <vector>
// | -------------------------------

namespace ENG
{
  class Batcher
  {
    public:
      static constexpr uint32_t MAX_QUADS     = 10000;
      static constexpr uint32_t MAX_VERTS     = MAX_QUADS * 4;
      static constexpr uint32_t MAX_INDICES   = MAX_QUADS * 6;
      static constexpr uint32_t MAX_TEXTURES  = 16;

      void Init();
      void Shutdown();

      void Begin(); /// First Frame
      void End();   /// Flush + DrawCall
    
    public:
      // API - Public
      void DrawTexture(const glm::vec2& pos, const glm::vec2& size, std::shared_ptr<RImage> texture, const glm::vec4& tint = {1,1,1,1});
      void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
      void DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color, uint32_t segments = 32);
      void DrawCircleOutLine(const glm::vec2& center, float radius, const glm::vec4& outcolor, float thickness = 1.0f);
      void DrawTriangle(const glm::vec2& pointA, const glm::vec2& pointB, const glm::vec2& pointC, const glm::vec4& color);
      void DrawLine(const glm::vec2& a, const glm::vec2& b, float thickness, const glm::vec4& color);
      void DrawPolygon(const std::vector<glm::vec2>& points, const glm::vec4& color);

    private:
      Shader shader;

      GLuint m_VAO, m_VBO, m_EBO;
      GLuint m_WhiteTexture = 0;

      Vertex*     m_VertexBufferBase  = nullptr;
      Vertex*     m_VertexBufferPtr   = nullptr;
      uint32_t*   m_IndexBufferBase   = nullptr;
      uint32_t*   m_IndexBufferPtr    = nullptr;

      uint32_t m_IndexCount       = 0;
      uint32_t m_VertexCount      = 0;
      
      // TextureSlot
      std::array<GLuint, MAX_TEXTURES> m_TextureSlots = {};
      uint32_t m_TextureSlotIndex = 1;    /// 0 = WhiteTexture
           
      void Flush();
      void StartBatch();
      float GetTextureIndex(const RImage& texture);
  };

}
