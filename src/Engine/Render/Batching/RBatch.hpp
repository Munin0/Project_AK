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
  /**
   * @brief Batch renderer: groups many draw calls into a few GPU draw calls.
   *
   * Batcher collects the quads, lines, circles, triangles, polygons, glyphs
   * and sprites submitted during a frame into a single vertex/index buffer
   * and sends them to the GPU in one go (see Flush()). Drawing many small
   * shapes this way is much cheaper than issuing a draw call for each one.
   *
   * Textures are referenced through a fixed set of texture slots, plus one
   * reserved unit for the TextureArray (atlas) sampler. The batch is
   * flushed early when it runs out of quads or texture slots.
   *
   * Typical frame:
   * @code
   * batcher.Begin();
   * batcher.DrawQuad({10.0f, 10.0f}, {50.0f, 50.0f}, {1, 0, 0, 1});
   * batcher.DrawTexture({100.0f, 10.0f}, {64.0f, 64.0f}, texture);
   * batcher.End();
   * @endcode
   *
   * @note An OpenGL context must be current when using the Batcher. Init()
   *       must be called before drawing and Shutdown() before the context
   *       is destroyed.
   */
  class Batcher
  {
    public:
      /// @brief Maximum number of quads per batch.
      static constexpr uint32_t MAX_QUADS     = 10000;

      /// @brief Maximum number of vertices per batch (4 per quad).
      static constexpr uint32_t MAX_VERTS     = MAX_QUADS * 4;

      /// @brief Maximum number of indices per batch (6 per quad).
      static constexpr uint32_t MAX_INDICES   = MAX_QUADS * 6;

      /// @brief Number of texture slots available in a batch. Slot 0 is
      ///        always the white texture, so 15 slots remain for other textures.
      static constexpr uint32_t MAX_TEXTURES  = 16;

      /// @brief Reserved texture unit for the TextureArray/atlas sampler.
      static constexpr uint32_t ARRAY_TEXTURE_UNIT = MAX_TEXTURES;

      /**
       * @brief Creates the GPU resources of the batcher.
       *
       * Creates the buffers, the white texture and the frame data buffer,
       * and prepares the batch for the first frame.
       */
      void Init();

      /**
       * @brief Releases the GPU resources of the batcher.
       */
      void Shutdown();

      /**
       * @brief Starts a frame.
       *
       * Must be called once at the start of a frame, before any draw call.
       */
      void Begin();

      /**
       * @brief Ends a frame.
       *
       * Flushes everything that is pending in the batch and issues the
       * draw call.
       */
      void End();

      /**
       * @brief Sets the camera used to build the view-projection matrix.
       *
       * The camera is not owned by the Batcher and must outlive it.
       *
       * @param[in] camera Camera to use.
       */
      void SetCamera2D(ENG::Camera2D* camera);

      /**
       * @brief Returns the camera used by the batcher.
       * @return Pointer to the camera set with SetCamera2D().
       */
      Camera2D* GetCamera2D() const;

    public:
      // Internal drawing API, normally used through Drawer and the Object classes.

      /**
       * @brief Draws a textured quad.
       *
       * @param[in] pos        Position of the quad.
       * @param[in] size       Size of the quad: { width, height }.
       * @param[in] texture    Texture to draw.
       * @param[in] tint       Color multiplied with the texture. Defaults to white (no tint).
       * @param[in] effectMode Effect applied by the shader (see Vertex::EffectMode).
       *                       Defaults to 0 (normal).
       */
      void DrawTexture(const glm::vec2& pos, const glm::vec2& size, std::shared_ptr<RImage> texture, const glm::vec4& tint = {1,1,1,1}, float effectMode = 0.0f);

      /**
       * @brief Draws a glyph (a character) from a font atlas.
       *
       * @param[in] pos        Position of the glyph.
       * @param[in] size       Size of the glyph: { width, height }.
       * @param[in] fontAtlas  Atlas texture of the font.
       * @param[in] uvMin      UV coordinates of one corner of the glyph inside the atlas.
       * @param[in] uvMax      UV coordinates of the opposite corner of the glyph inside the atlas.
       * @param[in] tint       Color multiplied with the glyph. Defaults to white (no tint).
       * @param[in] effectMode Effect applied by the shader (see Vertex::EffectMode).
       *                       Defaults to 0 (normal).
       */
      void DrawGlyph(const glm::vec2& pos, const glm::vec2& size, std::shared_ptr<RImage> fontAtlas, const glm::vec2& uvMin, const glm::vec2& uvMax, const glm::vec4& tint = {1,1,1,1}, float effectMode = 0.0f);

      /**
       * @brief Draws a sprite from a layer of the TextureArray (atlas).
       *
       * @param[in] pos        Position of the sprite.
       * @param[in] size       Size of the sprite: { width, height }.
       * @param[in] layer      Layer of the TextureArray that contains the atlas.
       * @param[in] uvMin      UV coordinates of one corner of the sprite inside the layer.
       * @param[in] uvMax      UV coordinates of the opposite corner of the sprite inside the layer.
       * @param[in] tint       Color multiplied with the sprite. Defaults to white (no tint).
       * @param[in] effectMode Effect applied by the shader (see Vertex::EffectMode).
       *                       Defaults to 0 (normal).
       */
      void DrawAtlasSprite(const glm::vec2& pos, const glm::vec2& size, int layer, const glm::vec2& uvMin, const glm::vec2& uvMax, const glm::vec4& tint = {1,1,1,1}, float effectMode = 0.0f);

      /**
       * @brief Draws a solid-color quad.
       *
       * @param[in] pos   Position of the quad.
       * @param[in] size  Size of the quad: { width, height }.
       * @param[in] color Color of the quad.
       */
      void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);

      /**
       * @brief Draws the outline of a quad.
       *
       * @param[in] x        X position of the quad.
       * @param[in] y        Y position of the quad.
       * @param[in] w        Width of the quad.
       * @param[in] h        Height of the quad.
       * @param[in] color    Color of the outline.
       * @param[in] thicknes Thickness of the outline. Defaults to 1.0.
       */
      void DrawQuadOutline(float x, float y, float w, float h, const Color& color, float thicknes = 1.0f);

      /**
       * @brief Draws a filled circle.
       *
       * @param[in] center   Center of the circle.
       * @param[in] radius   Radius of the circle.
       * @param[in] color    Fill color.
       * @param[in] segments Number of segments used to approximate the circle. Defaults to 32.
       */
      void DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color, uint32_t segments = 32);

      /**
       * @brief Draws the outline of a circle.
       *
       * @param[in] center    Center of the circle.
       * @param[in] radius    Radius of the circle.
       * @param[in] outcolor  Color of the outline.
       * @param[in] thickness Thickness of the outline. Defaults to 1.0.
       */
      void DrawCircleOutLine(const glm::vec2& center, float radius, const glm::vec4& outcolor, float thickness = 1.0f);

      /**
       * @brief Draws a filled triangle.
       *
       * @param[in] pointA First vertex.
       * @param[in] pointB Second vertex.
       * @param[in] pointC Third vertex.
       * @param[in] color  Fill color.
       */
      void DrawTriangle(const glm::vec2& pointA, const glm::vec2& pointB, const glm::vec2& pointC, const glm::vec4& color);

      /**
       * @brief Draws a line between two points.
       *
       * @param[in] a         Start point.
       * @param[in] b         End point.
       * @param[in] thickness Thickness of the line.
       * @param[in] color     Color of the line.
       */
      void DrawLine(const glm::vec2& a, const glm::vec2& b, float thickness, const glm::vec4& color);

      /**
       * @brief Draws a filled polygon.
       *
       * @param[in] points Vertices of the polygon, in order.
       * @param[in] color  Fill color.
       */
      void DrawPolygon(const std::vector<glm::vec2>& points, const glm::vec4& color);

      /**
       * @brief Sets the shader (material) used to draw.
       *
       * @param[in] _Material Shader to draw with.
       */
      void SetMaterial(Shader* _Material);

    private:
      /**
       * @brief Per-frame data sent to the GPU in a uniform buffer.
       */
      struct FrameDataGPU
      {
        /// @brief View-projection matrix of the camera.
        glm::mat4 ViewProjection;

        /// @brief Time value made available to the shaders.
        float     Time;

        /// @brief Width of the screen, in pixels.
        float     ScreenWidth;

        /// @brief Height of the screen, in pixels.
        float     ScreenHeight;
      };

      /**
       * @brief Uploads the per-frame data to the GPU.
       *
       * @param[in] viewProj View-projection matrix to send.
       */
      void UpdateFrameData(const glm::mat4& viewProj);

      /// @brief Shader currently used to draw.
      Shader* shader;

      /// @brief Default shader of the batcher. nullptr until created.
      Shader* m_DefaultShader = nullptr;

      /// @brief Projection matrix.
      glm::mat4 m_proj;

      /// @brief OpenGL vertex array, vertex buffer and index buffer of the batch.
      GLuint m_VAO, m_VBO, m_EBO;

      /// @brief OpenGL ID of the white texture (slot 0). 0 until created.
      GLuint m_WhiteTexture = 0;

      /// @brief OpenGL ID of the TextureArray. 0 until set.
      GLuint m_ArrayTextureID = 0;

      /// @brief OpenGL uniform buffer with the per-frame data. 0 until created.
      GLuint m_FrameUBO = 0;

      /// @brief Start of the CPU-side vertex buffer of the batch.
      Vertex*     m_VertexBufferBase  = nullptr;

      /// @brief Next free position in the CPU-side vertex buffer.
      Vertex*     m_VertexBufferPtr   = nullptr;

      /// @brief Start of the CPU-side index buffer of the batch.
      uint32_t*   m_IndexBufferBase   = nullptr;

      /// @brief Next free position in the CPU-side index buffer.
      uint32_t*   m_IndexBufferPtr    = nullptr;

      /// @brief Number of indices in the current batch.
      uint32_t m_IndexCount       = 0;

      /// @brief Number of vertices in the current batch.
      uint32_t m_VertexCount      = 0;

      /// @brief OpenGL texture IDs bound to each texture slot of the batch.
      std::array<GLuint, MAX_TEXTURES> m_TextureSlots = {};

      /// @brief Index of the next free texture slot. Starts at 1, because
      ///        slot 0 is the white texture.
      uint32_t m_TextureSlotIndex = 1;

      /**
       * @brief Sends the current batch to the GPU and draws it.
       */
      void Flush();

      /**
       * @brief Resets the batch so new geometry can be added.
       */
      void StartBatch();

      /**
       * @brief Returns the texture slot to use for a texture.
       *
       * @param[in] texture Texture to look up.
       * @return The slot of the texture in the current batch, as a float
       *         (ready to be stored in Vertex::TexIndex).
       */
      float GetTextureIndex(const RImage& texture);

      /// @brief Camera used to build the view-projection matrix. Not owned.
      Camera2D* m_camera;
  };
}
