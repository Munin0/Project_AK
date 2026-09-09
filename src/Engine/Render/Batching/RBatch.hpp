// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/Camera/Camera2D.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Image/RImage.hpp"
#include "Engine/Render/Shaders/RVertex.hpp"
#include "Engine/Render/Shaders/RShader.hpp"
// | -------------------------------
#include "glm/detail/qualifier.hpp"
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
      static constexpr uint32_t ARRAY_TEXTURE_UNIT = MAX_TEXTURES; // Reserved unit for the TextureArray/atlas sampler

      void Init();
      void Shutdown();

      void Begin(); /// First Frame
      void End();   /// Flush + DrawCall
 
      void SetCamera2D(ENG::Camera2D* camera);
      Camera2D* GetCamera2D() const;

    public:
      // API Private
      void DrawTexture(const glm::vec2& pos, const glm::vec2& size, std::shared_ptr<RImage> texture, const glm::vec4& tint = {1,1,1,1}, float effectMode = 0.0f);
      void DrawGlyph(const glm::vec2& pos, const glm::vec2& size, std::shared_ptr<RImage> fontAtlas, const glm::vec2& uvMin, const glm::vec2& uvMax, const glm::vec4& tint = {1,1,1,1});
      void DrawAtlasSprite(const glm::vec2& pos, const glm::vec2& size, int layer, const glm::vec2& uvMin, const glm::vec2& uvMax, const glm::vec4& tint = {1,1,1,1}, float effectMode = 0.0f);
      void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
      void DrawQuadOutline(float x, float y, float w, float h, const Color& color, float thicknes = 1.0f);
      void DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color, uint32_t segments = 32);
      void DrawCircleOutLine(const glm::vec2& center, float radius, const glm::vec4& outcolor, float thickness = 1.0f);
      void DrawTriangle(const glm::vec2& pointA, const glm::vec2& pointB, const glm::vec2& pointC, const glm::vec4& color);
      void DrawLine(const glm::vec2& a, const glm::vec2& b, float thickness, const glm::vec4& color);
      void DrawPolygon(const std::vector<glm::vec2>& points, const glm::vec4& color);

      void SetMaterial(Shader* _Material);

    private:
      // Struct for FrameData for GPU drawin
      struct FrameDataGPU
      {
        glm::mat4 ViewProjection;
        float     Time;
        float     ScreenWidth;
        float     ScreenHeight;
      };

      void UpdateFrameData(const glm::mat4& viewProj);

      Shader* shader;
      Shader* m_DefaultShader = nullptr;

      glm::mat4 m_proj;

      GLuint m_VAO, m_VBO, m_EBO;
      GLuint m_WhiteTexture = 0;
      GLuint m_ArrayTextureID = 0;
      GLuint m_FrameUBO = 0;
    
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

      Camera2D* m_camera;
  };

}
