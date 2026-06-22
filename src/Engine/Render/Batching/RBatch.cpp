/// | ------------------------------------ |
#include "RBatch.hpp"
/// | ------------------------------------ |
#include "Engine/Render/Camera/Camera2D.hpp"
#include "Engine/Render/Render.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Image/RImage.hpp"
#include "Engine/Render/Shaders/RVertex.hpp"
#include "Engine/Render/Shaders/RShader.hpp"
#include "Engine/Utils/Path.hpp"
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
#include <vector>
/// | ------------------------------------ |

namespace ENG
{
  void Batcher::Init()
  {
    /// Starting buffer on CPU
    m_VertexBufferBase = new Vertex[MAX_VERTS];
    m_IndexBufferBase  = new uint32_t[MAX_INDICES];

    // EBO
    glGenBuffers(1, &m_EBO);

    // VAO
    glGenVertexArrays(1,&m_VAO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    
    // VBO Dinamyc -> Each frame change
    glGenBuffers(1,&m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTS * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    // Vertex attributes
    // | -> Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex), (void*)offsetof(Vertex, Position));

    // | -> Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Color));

    // | -> TextCord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCord));

    // | -> TexIndex
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexIndex));

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDICES * sizeof(uint32_t),nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);

    // White Texture 1x1
    glGenTextures(1, &m_WhiteTexture);
    glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    uint32_t white = 0xFFFFFFFF;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, &white);
    glBindTexture(GL_TEXTURE_2D,0);
    m_TextureSlots[0] = m_WhiteTexture;


    // Shader
    auto& path = Path::Get();
    
    auto p = Path::Get().ShadersPath;
    auto _vPath = path.ReadFile(p /  "gl/texture.vs" );
    auto _fPath = path.ReadFile(p /  "gl/texture.fs" );

    this->shader = Shader( _vPath, _fPath);
    glUseProgram(this->shader.GetProgram());

    // Asign samplers one time
    int samplers[MAX_TEXTURES];
    for (int i=0; i < MAX_TEXTURES; i++)
      samplers[i] = i;
    glUniform1iv(glGetUniformLocation(this->shader.GetProgram(), "u_Textures"), MAX_TEXTURES, samplers);

  }

  void Batcher::Shutdown()
  {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteTextures(1, &m_WhiteTexture);
    glDeleteProgram(this->shader.GetProgram());
    delete [] m_VertexBufferBase;
    delete [] m_IndexBufferBase;
  }

  void Batcher::Begin()
  {
    StartBatch();
  }
  
  void Batcher::End()
  {
    Flush();
  }

  void Batcher::SetCamera2D(ENG::Camera2D* camera)
  {
    m_camera = camera; 
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

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    
    /// Load Matrix
    glm::mat4 proj;
    if(m_camera)
    {
      proj = m_camera->GetViewProjectionMatrix();
    }
    else 
    {
      Vector2 wS = Render::Get().GetScreenSize();
      proj = glm::ortho(0.0f, wS.x,wS.y,0.0f, -1.0f, 1.0f);
    }

    glUniformMatrix4fv(
      glGetUniformLocation(shader.GetProgram(), "u_ViewProjection"),
    1 , GL_FALSE, &proj[0][0]
    );

    // Push vertex to VRam
    uint32_t dataSize = (uint32_t)((uint8_t*)m_VertexBufferPtr - (uint8_t*)m_VertexBufferBase);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, m_VertexBufferBase);
    
    uint32_t indexSize = (uint32_t)((uint8_t*)m_IndexBufferPtr - (uint8_t*)m_IndexBufferBase);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexSize, m_IndexBufferBase);

    for( uint32_t i=0; i<m_TextureSlotIndex; i++)
    // Bind textures on slots
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, m_TextureSlots[i]);
    }
    glUseProgram(shader.GetProgram());
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
    m_VertexBufferPtr->Position  = {pos.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color     = color;
    m_VertexBufferPtr->TexCord   = {0.0f, 0.0f};
    m_VertexBufferPtr->TexIndex  = 0.0f;
    m_VertexBufferPtr++;

    // Bottom-right
    m_VertexBufferPtr->Position  = {pos.x + size.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color     = color;
    m_VertexBufferPtr->TexCord   = {1.0f, 0.0f};
    m_VertexBufferPtr->TexIndex  = 0.0f;
    m_VertexBufferPtr++;

    // Top-right
    m_VertexBufferPtr->Position  = {pos.x + size.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color     = color;
    m_VertexBufferPtr->TexCord   = {1.0f, 1.0f};
    m_VertexBufferPtr->TexIndex  = 0.0f;
    m_VertexBufferPtr++;

    // Top-left
    m_VertexBufferPtr->Position  = {pos.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color     = color;
    m_VertexBufferPtr->TexCord   = {0.0f, 1.0f};
    m_VertexBufferPtr->TexIndex  = 0.0f;
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

  void Batcher::DrawTexture(const glm::vec2& pos, const glm::vec2& size, std::shared_ptr<RImage> texture, const glm::vec4& tint)
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
    m_VertexBufferPtr->Position  = {pos.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color     = tint;
    m_VertexBufferPtr->TexCord   = {0.0f, 0.0f};
    m_VertexBufferPtr->TexIndex  = textureIndex;
    m_VertexBufferPtr++;

    // Bottom-right
    m_VertexBufferPtr->Position  = {pos.x + size.x, pos.y, 0.0f};
    m_VertexBufferPtr->Color     = tint;
    m_VertexBufferPtr->TexCord   = {1.0f, 0.0f};
    m_VertexBufferPtr->TexIndex  = textureIndex;
    m_VertexBufferPtr++;

    // Top-right
    m_VertexBufferPtr->Position  = {pos.x + size.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color     = tint;
    m_VertexBufferPtr->TexCord   = {1.0f, 1.0f};
    m_VertexBufferPtr->TexIndex  = textureIndex;
    m_VertexBufferPtr++;

    // Top-left
    m_VertexBufferPtr->Position  = {pos.x, pos.y + size.y, 0.0f};
    m_VertexBufferPtr->Color     = tint;
    m_VertexBufferPtr->TexCord   = {0.0f, 1.0f};
    m_VertexBufferPtr->TexIndex  = textureIndex;
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
      m_VertexBufferPtr->Position = {center.x, center.y, 0.0f};
      m_VertexBufferPtr->Color    = color;
      m_VertexBufferPtr->TexCord  = {0.5f, 0.5f}; // Center of a dummy texture
      m_VertexBufferPtr->TexIndex = textureIndex;
      m_VertexBufferPtr++;

      // Point 1 on circumference
      m_VertexBufferPtr->Position = {p1.x, p1.y, 0.0f};
      m_VertexBufferPtr->Color    = color;
      m_VertexBufferPtr->TexCord  = {0.5f + 0.5f * std::cos(angle1), 0.5f + 0.5f * std::sin(angle1)};
      m_VertexBufferPtr->TexIndex = textureIndex;
      m_VertexBufferPtr++;

      // Point 2 on circumference
      m_VertexBufferPtr->Position = {p2.x, p2.y, 0.0f};
      m_VertexBufferPtr->Color    = color;
      m_VertexBufferPtr->TexCord  = {0.5f + 0.5f * std::cos(angle2), 0.5f + 0.5f * std::sin(angle2)};
      m_VertexBufferPtr->TexIndex = textureIndex;
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
    m_VertexBufferPtr->Position  = {pointA.x, pointA.y, 0.0f};
    m_VertexBufferPtr->Color     = color;
    m_VertexBufferPtr->TexCord   = {0.0f, 0.0f};
    m_VertexBufferPtr->TexIndex  = textureIndex;
    m_VertexBufferPtr++;

    // Point B
    m_VertexBufferPtr->Position  = {pointB.x, pointB.y, 0.0f};
    m_VertexBufferPtr->Color     = color;
    m_VertexBufferPtr->TexCord   = {0.5f, 1.0f};
    m_VertexBufferPtr->TexIndex  = textureIndex;
    m_VertexBufferPtr++;

    // Point C
    m_VertexBufferPtr->Position  = {pointC.x, pointC.y, 0.0f};
    m_VertexBufferPtr->Color     = color;
    m_VertexBufferPtr->TexCord   = {1.0f, 0.0f};
    m_VertexBufferPtr->TexIndex  = textureIndex;
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
    m_VertexBufferPtr->Position  = {p1.x, p1.y, 0.0f};
    m_VertexBufferPtr->Color     = color;
    m_VertexBufferPtr->TexCord   = {0.0f, 0.0f};
    m_VertexBufferPtr->TexIndex  = textureIndex;
    m_VertexBufferPtr++;

    // Bottom-right (p2)
    m_VertexBufferPtr->Position  = {p2.x, p2.y, 0.0f};
    m_VertexBufferPtr->Color     = color;
    m_VertexBufferPtr->TexCord   = {1.0f, 0.0f};
    m_VertexBufferPtr->TexIndex  = textureIndex;
    m_VertexBufferPtr++;

    // Top-right (p3)
    m_VertexBufferPtr->Position  = {p3.x, p3.y, 0.0f};
    m_VertexBufferPtr->Color     = color;
    m_VertexBufferPtr->TexCord   = {1.0f, 1.0f};
    m_VertexBufferPtr->TexIndex  = textureIndex;
    m_VertexBufferPtr++;

    // Top-left (p4)
    m_VertexBufferPtr->Position  = {p4.x, p4.y, 0.0f};
    m_VertexBufferPtr->Color     = color;
    m_VertexBufferPtr->TexCord   = {0.0f, 1.0f};
    m_VertexBufferPtr->TexIndex  = textureIndex;
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
        m_VertexBufferPtr->Position = {p.x, p.y, 0.0f};
        m_VertexBufferPtr->Color    = color;
        m_VertexBufferPtr->TexCord  = {0.0f, 0.0f};
        m_VertexBufferPtr->TexIndex = 0.0f;
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

