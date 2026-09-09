/// | ------------------------------------ |
#include "RBatch.hpp"
/// | ------------------------------------ |
#include "Engine/Render/Camera/Camera2D.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Image/RImage.hpp"
#include "Engine/Render/Shaders/RVertex.hpp"
#include "Engine/Render/Shaders/RShader.hpp"
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
    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position));
    glVertexArrayAttribBinding(m_VAO, 0, 0);

    // | -> Color
    glEnableVertexArrayAttrib(m_VAO, 1);
    glVertexArrayAttribFormat(m_VAO, 1, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, Color));
    glVertexArrayAttribBinding(m_VAO, 1, 0);

    // | -> TextCord
    glEnableVertexArrayAttrib(m_VAO, 2);
    glVertexArrayAttribFormat(m_VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCord));
    glVertexArrayAttribBinding(m_VAO, 2, 0);

    // | -> TexIndex
    glEnableVertexArrayAttrib(m_VAO, 3);
    glVertexArrayAttribFormat(m_VAO, 3, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexIndex));
    glVertexArrayAttribBinding(m_VAO, 3, 0);

    // | -> ArrayLayer
    glEnableVertexArrayAttrib(m_VAO, 4);
    glVertexArrayAttribFormat(m_VAO, 4, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, ArrayLayer));
    glVertexArrayAttribBinding(m_VAO, 4, 0);

    // | -> ArrayLayer
    glEnableVertexArrayAttrib(m_VAO, 5);
    glVertexArrayAttribFormat(m_VAO, 5, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, EffectMode));
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
    const glm::mat4 proj = m_camera ? m_camera->GetViewProjectionMatrix() : m_proj;

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

  void Batcher::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
  {
    if (m_IndexCount >= MAX_INDICES)
    {
      Flush();
      StartBatch();
    }

    uint32_t base = m_VertexCount;

    // Bottom-left
    m_VertexBufferPtr->Position   = {pos.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color      = color;
    m_VertexBufferPtr->TexCord    = {0.0f, 0.0f};
    m_VertexBufferPtr->TexIndex   = 0.0f;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Bottom-right
    m_VertexBufferPtr->Position   = {pos.x + size.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color      = color;
    m_VertexBufferPtr->TexCord    = {1.0f, 0.0f};
    m_VertexBufferPtr->TexIndex   = 0.0f;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Top-right
    m_VertexBufferPtr->Position   = {pos.x + size.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color      = color;
    m_VertexBufferPtr->TexCord    = {1.0f, 1.0f};
    m_VertexBufferPtr->TexIndex   = 0.0f;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Top-left
    m_VertexBufferPtr->Position   = {pos.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color      = color;
    m_VertexBufferPtr->TexCord    = {0.0f, 1.0f};
    m_VertexBufferPtr->TexIndex   = 0.0f;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    *m_IndexBufferPtr++ = base + 0;
    *m_IndexBufferPtr++ = base + 1;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 3;
    *m_IndexBufferPtr++ = base + 0;

    m_VertexCount += 4;
    m_IndexCount += 6;
  }

  void Batcher::DrawQuadOutline(float x, float y, float w, float h, const Color& color, float thicknes)
  {
    glm::vec4 outColor = {color.r, color.g, color.b, color.a};

    DrawQuad({x, y}, {w, thicknes}, outColor);                                                // Top
    DrawQuad({x, y + h - thicknes}, {w, thicknes}, outColor);                              // Bottom
    DrawQuad({x, y + thicknes}, {thicknes, h - thicknes * 2}, outColor);                   // Left
    DrawQuad({x + w - thicknes, y + thicknes}, {thicknes, h - thicknes * 2}, outColor); // Right
  }
  
  void Batcher::DrawGlyph(const glm::vec2& pos, const glm::vec2& size, 
      std::shared_ptr<RImage> fontAtlas, const glm::vec2& uvMin, const glm::vec2& uvMax, const glm::vec4& tint)
  {
    if (m_IndexCount >= MAX_INDICES)
    {
        Flush();
        StartBatch();
    }

    const RImage* tex = fontAtlas.get();
    float textureIndex = GetTextureIndex(*tex);

    uint32_t base = m_VertexCount;

    // Bottom-left
    m_VertexBufferPtr->Position   = {pos.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {uvMin.x, uvMin.y};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Bottom-right
    m_VertexBufferPtr->Position   = {pos.x + size.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {uvMax.x, uvMin.y};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Top-right
    m_VertexBufferPtr->Position   = {pos.x + size.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {uvMax.x, uvMax.y};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Top-left
    m_VertexBufferPtr->Position   = {pos.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {uvMin.x, uvMax.y};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    *m_IndexBufferPtr++ = base + 0;
    *m_IndexBufferPtr++ = base + 1;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 3;
    *m_IndexBufferPtr++ = base + 0;

    m_VertexCount += 4;
    m_IndexCount  += 6;
  }

  void Batcher::DrawTexture(const glm::vec2& pos, const glm::vec2& size, std::shared_ptr<RImage> texture, const glm::vec4& tint, float effectMode)
  {
    if (m_IndexCount >= MAX_INDICES)
    {
      Flush();
      StartBatch();
    }

    const RImage* tex = texture.get();
    float textureIndex = GetTextureIndex(*tex);

    uint32_t base = m_VertexCount;

    // Bottom-left
    m_VertexBufferPtr->Position   = {pos.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {0.0f, 0.0f};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Bottom-right
    m_VertexBufferPtr->Position   = {pos.x + size.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {1.0f, 0.0f};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Top-right
    m_VertexBufferPtr->Position   = {pos.x + size.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {1.0f, 1.0f};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Top-left
    m_VertexBufferPtr->Position   = {pos.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {0.0f, 1.0f};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    *m_IndexBufferPtr++ = base + 0;
    *m_IndexBufferPtr++ = base + 1;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 3;
    *m_IndexBufferPtr++ = base + 0;

    m_VertexCount += 4;
    m_IndexCount  += 6;
  }

  void Batcher::DrawAtlasSprite(const glm::vec2& pos, const glm::vec2& size, int layer, const glm::vec2& uvMin, const glm::vec2& uvMax, const glm::vec4& tint, float effectMode)
  {
    if (m_IndexCount >= MAX_INDICES)
    {
      Flush();
      StartBatch();
    }

    uint32_t base = m_VertexCount;
    float arrayLayer = (float)layer;

    // Bottom-left
    m_VertexBufferPtr->Position   = {pos.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {uvMin.x, uvMin.y};
    m_VertexBufferPtr->TexIndex   = 0.0f;
    m_VertexBufferPtr->ArrayLayer = arrayLayer;
    m_VertexBufferPtr++;

    // Bottom-right
    m_VertexBufferPtr->Position   = {pos.x + size.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {uvMax.x, uvMin.y};
    m_VertexBufferPtr->TexIndex   = 0.0f;
    m_VertexBufferPtr->ArrayLayer = arrayLayer;
    m_VertexBufferPtr++;

    // Top-right
    m_VertexBufferPtr->Position   = {pos.x + size.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {uvMax.x, uvMax.y};
    m_VertexBufferPtr->TexIndex   = 0.0f;
    m_VertexBufferPtr->ArrayLayer = arrayLayer;
    m_VertexBufferPtr++;

    // Top-left
    m_VertexBufferPtr->Position   = {pos.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color      = tint;
    m_VertexBufferPtr->TexCord    = {uvMin.x, uvMax.y};
    m_VertexBufferPtr->TexIndex   = 0.0f;
    m_VertexBufferPtr->ArrayLayer = arrayLayer;
    m_VertexBufferPtr++;

    *m_IndexBufferPtr++ = base + 0;
    *m_IndexBufferPtr++ = base + 1;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 3;
    *m_IndexBufferPtr++ = base + 0;

    m_VertexCount += 4;
    m_IndexCount  += 6;
  }

  void Batcher::DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color, uint32_t segments)
  {
    if (m_IndexCount >= MAX_INDICES)
    {
      Flush();
      StartBatch();
    }

    float angleIncrement = 2.0f * glm::pi<float>() / static_cast<float>(segments);
    float textureIndex = 0.0f; // White Texture

    for (uint32_t i = 0; i < segments; ++i)
    {
      uint32_t base = m_VertexCount;

      float angle1 = static_cast<float>(i) * angleIncrement;
      float angle2 = static_cast<float>(i + 1) * angleIncrement;

      glm::vec2 p1 = {center.x + radius * std::cos(angle1), center.y + radius * std::sin(angle1)};
      glm::vec2 p2 = {center.x + radius * std::cos(angle2), center.y + radius * std::sin(angle2)};

      // Center vertex
      m_VertexBufferPtr->Position   = {center.x, center.y, 0.0f};
      m_VertexBufferPtr->Color      = color;
      m_VertexBufferPtr->TexCord    = {0.5f, 0.5f}; // Center of a dummy texture
      m_VertexBufferPtr->TexIndex   = textureIndex;
      m_VertexBufferPtr->ArrayLayer = -1.0f;
      m_VertexBufferPtr++;

      // Point 1 on circumference
      m_VertexBufferPtr->Position   = {p1.x, p1.y, 0.0f};
      m_VertexBufferPtr->Color      = color;
      m_VertexBufferPtr->TexCord    = {0.5f + 0.5f * std::cos(angle1), 0.5f + 0.5f * std::sin(angle1)};
      m_VertexBufferPtr->TexIndex   = textureIndex;
      m_VertexBufferPtr->ArrayLayer = -1.0f;
      m_VertexBufferPtr++;

      // Point 2 on circumference
      m_VertexBufferPtr->Position   = {p2.x, p2.y, 0.0f};
      m_VertexBufferPtr->Color      = color;
      m_VertexBufferPtr->TexCord    = {0.5f + 0.5f * std::cos(angle2), 0.5f + 0.5f * std::sin(angle2)};
      m_VertexBufferPtr->TexIndex   = textureIndex;
      m_VertexBufferPtr->ArrayLayer = -1.0f;
      m_VertexBufferPtr++;
      
      *m_IndexBufferPtr++ = base + 0;
      *m_IndexBufferPtr++ = base + 1;
      *m_IndexBufferPtr++ = base + 2;

      m_VertexCount += 3;
      m_IndexCount += 3; // Each segment adds one triangle

      if (m_IndexCount >= MAX_INDICES)
      {
        Flush();
        StartBatch();
      }
    }
  }

  void Batcher::DrawCircleOutLine(const glm::vec2& center, float radius, const glm::vec4& outcolor, float thickness)
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
    if (m_IndexCount >= MAX_INDICES)
    {
      Flush();
      StartBatch();
    }

    float textureIndex = 0.0f; // White Texture
    uint32_t base = m_VertexCount;

    // Point A
    m_VertexBufferPtr->Position   = {pointA.x, pointA.y, 0.0f};
    m_VertexBufferPtr->Color      = color;
    m_VertexBufferPtr->TexCord    = {0.0f, 0.0f};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Point B
    m_VertexBufferPtr->Position   = {pointB.x, pointB.y, 0.0f};
    m_VertexBufferPtr->Color      = color;
    m_VertexBufferPtr->TexCord    = {0.5f, 1.0f};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Point C
    m_VertexBufferPtr->Position   = {pointC.x, pointC.y, 0.0f};
    m_VertexBufferPtr->Color      = color;
    m_VertexBufferPtr->TexCord    = {1.0f, 0.0f};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    *m_IndexBufferPtr++ = base + 0;
    *m_IndexBufferPtr++ = base + 1;
    *m_IndexBufferPtr++ = base + 2;

    m_VertexCount += 3;
    m_IndexCount  += 3;  
  }

  void Batcher::DrawLine(const glm::vec2& a, const glm::vec2& b, float thickness, const glm::vec4& color)
  {
    if (m_IndexCount >= MAX_INDICES)
    {
      Flush();
      StartBatch();
    }

    Vector2 v = {a.x, a.y};
    Vector2 vb = {b.x,b.y};
    Vector2 r = v-vb;
    r.Normalize();

    glm::vec2 direction = {r.x,r.y};
    glm::vec2 normal    = glm::vec2(-direction.y, direction.x) * (thickness * 0.5f);

    // Vertices for the thick line quad
    glm::vec2 p1 = a - normal;
    glm::vec2 p2 = b - normal;
    glm::vec2 p3 = b + normal;
    glm::vec2 p4 = a + normal;

    float textureIndex = 0.0f; // White Texture
    uint32_t base = m_VertexCount; 

    // Bottom-left (p1)
    m_VertexBufferPtr->Position   = {p1.x, p1.y, 0.0f};
    m_VertexBufferPtr->Color      = color;
    m_VertexBufferPtr->TexCord    = {0.0f, 0.0f};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Bottom-right (p2)
    m_VertexBufferPtr->Position   = {p2.x, p2.y, 0.0f};
    m_VertexBufferPtr->Color      = color;
    m_VertexBufferPtr->TexCord    = {1.0f, 0.0f};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Top-right (p3)
    m_VertexBufferPtr->Position   = {p3.x, p3.y, 0.0f};
    m_VertexBufferPtr->Color      = color;
    m_VertexBufferPtr->TexCord    = {1.0f, 1.0f};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    // Top-left (p4)
    m_VertexBufferPtr->Position   = {p4.x, p4.y, 0.0f};
    m_VertexBufferPtr->Color      = color;
    m_VertexBufferPtr->TexCord    = {0.0f, 1.0f};
    m_VertexBufferPtr->TexIndex   = textureIndex;
    m_VertexBufferPtr->ArrayLayer = -1.0f;
    m_VertexBufferPtr++;

    *m_IndexBufferPtr++ = base + 0;
    *m_IndexBufferPtr++ = base + 1;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 2;
    *m_IndexBufferPtr++ = base + 3;
    *m_IndexBufferPtr++ = base + 0;

    m_VertexCount += 4;
    m_IndexCount += 6;
  }
  
  void Batcher::DrawPolygon(const std::vector<glm::vec2>& points, const glm::vec4& color)
  {
    if(points.size() < 3) return;
    
    uint32_t base = m_VertexCount;

    // Subir todos los vértices
    for(const auto& p : points)
    {
        m_VertexBufferPtr->Position   = {p.x, p.y, 0.0f};
        m_VertexBufferPtr->Color      = color;
        m_VertexBufferPtr->TexCord    = {0.0f, 0.0f};
        m_VertexBufferPtr->TexIndex   = 0.0f;
        m_VertexBufferPtr->ArrayLayer = -1.0f;
        m_VertexBufferPtr++;
        m_VertexCount++;
    }

    // Triangle Fan
    for(uint32_t i = 1; i < points.size() - 1; i++)
    {
        *m_IndexBufferPtr++ = base;
        *m_IndexBufferPtr++ = base + i;
        *m_IndexBufferPtr++ = base + i + 1;
        m_IndexCount += 3;
    }
  }

}
