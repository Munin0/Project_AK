/// | ------------------------------------ |
#include "Batch.hpp"
/// | ------------------------------------ |
#include "Engine/Render/Camera/Camera2D.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Render/Color/Color.hpp"
#include "Engine/Render/Image/Image.hpp"
#include "Engine/Render/Shaders/Vertex.hpp"
#include "Engine/Render/Shaders/Shader.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | ------------------------------------ |
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
/// | ------------------------------------ |
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <memory>
#include <string>
#include <vector>
/// | ------------------------------------ |

namespace ENG
{
  static void GLAPIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, const void*)
  {
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
 
    const char* severityStr =
      severity == GL_DEBUG_SEVERITY_HIGH   ? "HIGH"   :
      severity == GL_DEBUG_SEVERITY_MEDIUM ? "MEDIUM" :
      severity == GL_DEBUG_SEVERITY_LOW    ? "LOW"    : "NOTIFICATION";
 
    const char* typeStr =
      type == GL_DEBUG_TYPE_ERROR               ? "ERROR"      :
      type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR  ? "DEPRECATED" :
      type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR   ? "UB"         :
      type == GL_DEBUG_TYPE_PORTABILITY          ? "PORTABILITY": "OTHER";
 
    auto msg = TextAPI::Get().FormatText("[GL %s][%s] (id=%u) %s", severityStr, typeStr, id, message);
    LOG_ERROR(" | << " + msg);
  }

  void Batcher::Init()
  {
    /// Starting buffer on CPU
    m_VertexBufferBase = new Vertex[MAX_VERTS];
    m_IndexBufferBase  = new uint32_t[MAX_INDICES];
 
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

    // Create objects via DSA
    glCreateVertexArrays(1, &m_VAO);
    glCreateBuffers(1,&m_VBO);
    glCreateBuffers(1,&m_EBO);

    glNamedBufferData(m_VBO, MAX_VERTS * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    glNamedBufferData(m_EBO, MAX_INDICES * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

    // VBO to binding point 0 of the VAO (stripe = sizeof(Vertex))
    glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(m_VAO,m_EBO);

    // Vertex attributes
    // | -> Position
    glEnableVertexArrayAttrib(m_VAO, 0);
    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_position));
    glVertexArrayAttribBinding(m_VAO, 0, 0);

    // | -> Color
    glEnableVertexArrayAttrib(m_VAO, 1);
    glVertexArrayAttribFormat(m_VAO, 1, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_color));
    glVertexArrayAttribBinding(m_VAO, 1, 0);

    // | -> TextCoord
    glEnableVertexArrayAttrib(m_VAO, 2);
    glVertexArrayAttribFormat(m_VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_texCoord));
    glVertexArrayAttribBinding(m_VAO, 2, 0);

    // | -> TexIndex
    glEnableVertexArrayAttrib(m_VAO, 3);
    glVertexArrayAttribFormat(m_VAO, 3, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_texIndex));
    glVertexArrayAttribBinding(m_VAO, 3, 0);

    // | -> ArrayLayer
    glEnableVertexArrayAttrib(m_VAO, 4);
    glVertexArrayAttribFormat(m_VAO, 4, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_arrayLayer));
    glVertexArrayAttribBinding(m_VAO, 4, 0);

    // | -> ArrayLayer
    glEnableVertexArrayAttrib(m_VAO, 5);
    glVertexArrayAttribFormat(m_VAO, 5, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_effectMode));
    glVertexArrayAttribBinding(m_VAO, 5, 0);

    // White Texture 1x1 -> DSA + storage inmutrable (glTextureStorage2D)
    glCreateTextures(GL_TEXTURE_2D, 1, &m_WhiteTexture);
    glTextureParameteri(m_WhiteTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_WhiteTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_WhiteTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_WhiteTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    uint32_t white = 0xFFFFFFFF;
    glTextureStorage2D(m_WhiteTexture, 1, GL_RGBA8, 1, 1);
    glTextureSubImage2D(m_WhiteTexture, 0,0,0,1,1, GL_RGBA, GL_UNSIGNED_BYTE, &white);
    m_TextureSlots[0] = m_WhiteTexture;

    // Shader
    Services::Shaders().Load("gl/texture.fs","gl/texture.vs", "Default");
    this->shader = Services::Shaders().Get("Default");
    this->m_DefaultShader = this->shader;
    glUseProgram(this->shader->GetProgram());

    m_ArrayTextureID = Services::Assets().GetTextureArrayID();

    glCreateBuffers(1, &m_FrameUBO);
    glNamedBufferData(m_FrameUBO, sizeof(FrameDataGPU), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_FrameUBO);
  }

  void Batcher::UpdateFrameData(const glm::mat4& viewProj)
  {
    FrameDataGPU data;
    data.ViewProjection = viewProj;
    data.Time           = Services::Clock().GetTime();
 
    Vector2 wS = Render::Get().GetScreenSize();
    data.ScreenWidth  = wS.x;
    data.ScreenHeight = wS.y;
 
    glNamedBufferSubData(m_FrameUBO, 0, sizeof(FrameDataGPU), &data);
  }

  void Batcher::Shutdown()
  {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteTextures(1, &m_WhiteTexture);
    delete [] m_VertexBufferBase;
    delete [] m_IndexBufferBase;
  }

  void Batcher::Begin()
  {
    StartBatch();
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
  }
  
  void Batcher::End()
  {
    Flush();
    SetMaterial(nullptr);
  }

  void Batcher::SetMaterial(Shader* material)
  {
    Shader* target = material != nullptr ? material : m_DefaultShader;
    if(shader != target)
    {
      Flush();
      shader = target;
    }
  }

  void Batcher::SetCamera2D(ENG::Camera2D* camera)
  {
    // The camera is read at Flush(): draw what was batched with the old one first,
    // so a scene can switch to screen space (nullptr) for its UI in the same frame.
    if(camera != m_camera)
      Flush();
    m_camera = camera;
  }

  Camera2D* Batcher::GetCamera2D() const
  {
    return this->m_camera;
  }

  void Batcher::StartBatch()
  {
    m_VertexBufferPtr = m_VertexBufferBase;
    m_IndexBufferPtr  = m_IndexBufferBase;
    m_IndexCount = 0;
    m_VertexCount = 0;
    m_TextureSlotIndex = 1;
  }

  void Batcher::Flush()
  {
    if(m_IndexCount == 0) return;

    Vector2 wS = Render::Get().GetScreenSize();
    m_proj = glm::ortho(0.0f, wS.x,wS.y,0.0f, -1.0f, 1.0f);

    /// Load Matrix
    glm::mat4 proj = m_proj;
    if(m_camera != nullptr)
    {
      proj = m_camera->GetViewProjectionMatrix();
    }

    // Get default Shader, if we have a MaterialShader in coming, we change drawing
    Shader* activeShader = shader;
    glUseProgram(activeShader->GetProgram());
    // glUniformMatrix4fv( activeShader->GetViewProjectionLoc(),1 , GL_FALSE, &proj[0][0]);

    // Update all tempo
    UpdateFrameData(proj);

    // Push vertex to VRam
    uint32_t dataSize = (uint32_t)((uint8_t*)m_VertexBufferPtr - (uint8_t*)m_VertexBufferBase);
    glNamedBufferSubData(m_VBO, 0, dataSize, m_VertexBufferBase);
    
    uint32_t indexSize = (uint32_t)((uint8_t*)m_IndexBufferPtr - (uint8_t*)m_IndexBufferBase);
    glNamedBufferSubData(m_EBO, 0, indexSize, m_IndexBufferBase);

    /// Bind all textures
    for(uint32_t i = 0; i < m_TextureSlotIndex; i++)
      glBindTextureUnit(i, m_TextureSlots[i]);
    glBindTextureUnit(ARRAY_TEXTURE_UNIT, m_ArrayTextureID);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    StartBatch();  /// Reset
  }

  float Batcher::GetTextureIndex(const RImage& texture)
  {
    GLuint id = texture.GetID();
  
    // Search if exist on index;
    for(uint32_t i = 1; i< m_TextureSlotIndex; i++)
    {
      if(m_TextureSlots[i] == id)
        return (float)i;
    }

    // if Slots full -> Flush and reset
    if(m_TextureSlotIndex >= MAX_TEXTURES)
      Flush();

    m_TextureSlots[m_TextureSlotIndex] = id;
    return (float)m_TextureSlotIndex++;
  }

  bool Batcher::EnsureCapacity(uint32_t vertexCount, uint32_t indexCount)
  {
    // Geometry that would not fit even in an empty batch can never be drawn.
    if(vertexCount > MAX_VERTS || indexCount > MAX_INDICES)
      return false;

    if(m_VertexCount + vertexCount > MAX_VERTS || m_IndexCount + indexCount > MAX_INDICES)
      Flush();

    return true;
  }

  void Batcher::PushVertex(const glm::vec2& pos, const glm::vec4& color, const glm::vec2& uv, float texIndex, float arrayLayer, float effectMode)
  {
    m_VertexBufferPtr->m_position   = {pos.x, pos.y, 0.0f};
    m_VertexBufferPtr->m_color      = color;
    m_VertexBufferPtr->m_texCoord   = uv;
    m_VertexBufferPtr->m_texIndex   = texIndex;
    m_VertexBufferPtr->m_arrayLayer = arrayLayer;
    m_VertexBufferPtr->m_effectMode = effectMode;
    m_VertexBufferPtr++;
    m_VertexCount++;
  }

  void Batcher::PushQuadIndices(uint32_t base)
  {
    *m_IndexBufferPtr++ = base + 0;
    *m_IndexBufferPtr++ = base + 1;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 3;
    *m_IndexBufferPtr++ = base + 0;
    m_IndexCount += 6;
  }

  void Batcher::PushTriangleIndices(uint32_t a, uint32_t b, uint32_t c)
  {
    *m_IndexBufferPtr++ = a;
    *m_IndexBufferPtr++ = b;
    *m_IndexBufferPtr++ = c;
    m_IndexCount += 3;
  }

  void Batcher::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
  {
    EnsureCapacity(4, 6);

    uint32_t base = m_VertexCount;
    PushVertex({pos.x,          pos.y},          color, {0.0f, 0.0f}, 0.0f, -1.0f); // Bottom-left
    PushVertex({pos.x + size.x, pos.y},          color, {1.0f, 0.0f}, 0.0f, -1.0f); // Bottom-right
    PushVertex({pos.x + size.x, pos.y + size.y}, color, {1.0f, 1.0f}, 0.0f, -1.0f); // Top-right
    PushVertex({pos.x,          pos.y + size.y}, color, {0.0f, 1.0f}, 0.0f, -1.0f); // Top-left
    PushQuadIndices(base);
  }

  void Batcher::DrawQuadOutline(float x, float y, float w, float h, const Color& color, float thickness)
  {
    glm::vec4 outColor = {color.m_r, color.m_g, color.m_b, color.m_a};

    DrawQuad({x, y}, {w, thickness}, outColor);                                                // Top
    DrawQuad({x, y + h - thickness}, {w, thickness}, outColor);                              // Bottom
    DrawQuad({x, y + thickness}, {thickness, h - thickness * 2}, outColor);                   // Left
    DrawQuad({x + w - thickness, y + thickness}, {thickness, h - thickness * 2}, outColor); // Right
  }
  
  void Batcher::DrawGlyph(const glm::vec2& pos, const glm::vec2& size, 
      std::shared_ptr<RImage> fontAtlas, const glm::vec2& uvMin, const glm::vec2& uvMax, const glm::vec4& tint, float effectMode)
  {
    if(!fontAtlas)
      return;

    // Capacity first: GetTextureIndex() may flush too, and a flush after it
    // would invalidate the slot it returned.
    EnsureCapacity(4, 6);
    float textureIndex = GetTextureIndex(*fontAtlas);

    uint32_t base = m_VertexCount;
    PushVertex({pos.x,          pos.y},          tint, {uvMin.x, uvMin.y}, textureIndex, -1.0f, effectMode);
    PushVertex({pos.x + size.x, pos.y},          tint, {uvMax.x, uvMin.y}, textureIndex, -1.0f, effectMode);
    PushVertex({pos.x + size.x, pos.y + size.y}, tint, {uvMax.x, uvMax.y}, textureIndex, -1.0f, effectMode);
    PushVertex({pos.x,          pos.y + size.y}, tint, {uvMin.x, uvMax.y}, textureIndex, -1.0f, effectMode);
    PushQuadIndices(base);
  }

  void Batcher::DrawTexture(const glm::vec2& pos, const glm::vec2& size, std::shared_ptr<RImage> texture, const glm::vec4& tint, float effectMode)
  {
    if(!texture)
      return;

    // Capacity first: see DrawGlyph().
    EnsureCapacity(4, 6);
    float textureIndex = GetTextureIndex(*texture);

    uint32_t base = m_VertexCount;
    PushVertex({pos.x,          pos.y},          tint, {0.0f, 0.0f}, textureIndex, -1.0f, effectMode);
    PushVertex({pos.x + size.x, pos.y},          tint, {1.0f, 0.0f}, textureIndex, -1.0f, effectMode);
    PushVertex({pos.x + size.x, pos.y + size.y}, tint, {1.0f, 1.0f}, textureIndex, -1.0f, effectMode);
    PushVertex({pos.x,          pos.y + size.y}, tint, {0.0f, 1.0f}, textureIndex, -1.0f, effectMode);
    PushQuadIndices(base);
  }

  void Batcher::DrawAtlasSprite(const glm::vec2& pos, const glm::vec2& size, int layer, const glm::vec2& uvMin, const glm::vec2& uvMax, const glm::vec4& tint, float effectMode)
  {
    EnsureCapacity(4, 6);

    uint32_t base = m_VertexCount;
    float arrayLayer = static_cast<float>(layer);
    PushVertex({pos.x,          pos.y},          tint, {uvMin.x, uvMin.y}, 0.0f, arrayLayer, effectMode);
    PushVertex({pos.x + size.x, pos.y},          tint, {uvMax.x, uvMin.y}, 0.0f, arrayLayer, effectMode);
    PushVertex({pos.x + size.x, pos.y + size.y}, tint, {uvMax.x, uvMax.y}, 0.0f, arrayLayer, effectMode);
    PushVertex({pos.x,          pos.y + size.y}, tint, {uvMin.x, uvMax.y}, 0.0f, arrayLayer, effectMode);
    PushQuadIndices(base);
  }

  void Batcher::DrawAtlasIsometric(int atlasLayer, const UVRect& rect, const glm::vec2& pos, const glm::vec2& size, float offset)
  {
    glm::vec2 drawPos = pos - glm::vec2(0.0f, offset);
    DrawAtlasSprite(drawPos, size, atlasLayer, rect.m_uvMin, rect.m_uvMax);
  }

  void Batcher::DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color, uint32_t segments)
  {
    if(segments < 3)
      return;

    float angleIncrement = 2.0f * glm::pi<float>() / static_cast<float>(segments);

    // One triangle (3 vertices, 3 indices) per segment. The capacity is checked
    // per segment, so a circle with many segments may be split across flushes.
    for (uint32_t i = 0; i < segments; ++i)
    {
      EnsureCapacity(3, 3);

      float angle1 = static_cast<float>(i) * angleIncrement;
      float angle2 = static_cast<float>(i + 1) * angleIncrement;
      float c1 = std::cos(angle1), s1 = std::sin(angle1);
      float c2 = std::cos(angle2), s2 = std::sin(angle2);

      uint32_t base = m_VertexCount;
      PushVertex(center,                                        color, {0.5f, 0.5f},                         0.0f, -1.0f);
      PushVertex({center.x + radius * c1, center.y + radius * s1}, color, {0.5f + 0.5f * c1, 0.5f + 0.5f * s1}, 0.0f, -1.0f);
      PushVertex({center.x + radius * c2, center.y + radius * s2}, color, {0.5f + 0.5f * c2, 0.5f + 0.5f * s2}, 0.0f, -1.0f);
      PushTriangleIndices(base, base + 1, base + 2);
    }
  }

  void Batcher::DrawCircleOutline(const glm::vec2& center, float radius, const glm::vec4& outcolor, float thickness)
  {
    uint32_t segments = 32;
    float angleIncrement = 2.0f * glm::pi<float>() / static_cast<float>(segments);

    for (uint32_t i = 0; i < segments; ++i)
    {
      float angle1 = static_cast<float>(i) * angleIncrement;
      float angle2 = static_cast<float>(i + 1) * angleIncrement;

      glm::vec2 p1 = {center.x + radius * std::cos(angle1), center.y + radius * std::sin(angle1)};
      glm::vec2 p2 = {center.x + radius * std::cos(angle2), center.y + radius * std::sin(angle2)};

      DrawLine(p1, p2, thickness, outcolor);
    }
  }

  void Batcher::DrawTriangle(const glm::vec2& pointA, const glm::vec2& pointB, const glm::vec2& pointC, const glm::vec4& color)
  {
    EnsureCapacity(3, 3);

    uint32_t base = m_VertexCount;
    PushVertex(pointA, color, {0.0f, 0.0f}, 0.0f, -1.0f);
    PushVertex(pointB, color, {0.5f, 1.0f}, 0.0f, -1.0f);
    PushVertex(pointC, color, {1.0f, 0.0f}, 0.0f, -1.0f);
    PushTriangleIndices(base, base + 1, base + 2);
  }

  void Batcher::DrawLine(const glm::vec2& a, const glm::vec2& b, float thickness, const glm::vec4& color)
  {
    EnsureCapacity(4, 6);

    Vector2 v = {a.x, a.y};
    Vector2 vb = {b.x,b.y};
    Vector2 r = v-vb;
    r.Normalize();

    glm::vec2 direction = {r.x,r.y};
    glm::vec2 normal    = glm::vec2(-direction.y, direction.x) * (thickness * 0.5f);

    uint32_t base = m_VertexCount;
    PushVertex(a - normal, color, {0.0f, 0.0f}, 0.0f, -1.0f); // Bottom-left
    PushVertex(b - normal, color, {1.0f, 0.0f}, 0.0f, -1.0f); // Bottom-right
    PushVertex(b + normal, color, {1.0f, 1.0f}, 0.0f, -1.0f); // Top-right
    PushVertex(a + normal, color, {0.0f, 1.0f}, 0.0f, -1.0f); // Top-left
    PushQuadIndices(base);
  }
  
  void Batcher::DrawPolygon(const std::vector<glm::vec2>& points, const glm::vec4& color)
  {
    if(points.size() < 3) return;

    // Triangle fan: n vertices and (n - 2) triangles. A fan cannot be split
    // across flushes, so all of it must fit in one batch.
    const auto vertexCount = static_cast<uint32_t>(points.size());
    const uint32_t indexCount = (vertexCount - 2) * 3;
    if(!EnsureCapacity(vertexCount, indexCount))
    {
      LOG_ERROR(" | << Batcher::DrawPolygon: too many points (" + std::to_string(vertexCount) + ")");
      return;
    }

    uint32_t base = m_VertexCount;
    for(const auto& p : points)
      PushVertex(p, color, {0.0f, 0.0f}, 0.0f, -1.0f);

    for(uint32_t i = 1; i < vertexCount - 1; i++)
      PushTriangleIndices(base, base + i, base + i + 1);
  }

}
