/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include <glad/glad.h>
/// | ------------------------------------ |
#include <cstdint>
/// | ------------------------------------ |

namespace ENG
{
  class TextureArray
  {
  public:
    TextureArray() = default;
    ~TextureArray();

    // No copy, only GPU ownership
    TextureArray(const TextureArray&) = delete;
    TextureArray& operator=(const TextureArray&) = delete;

    // Move ownership (Optional) 
    TextureArray(TextureArray&& other) noexcept;
    TextureArray& operator=(TextureArray&& other) noexcept;

    // Make space on GPU for layer (size cap)
    bool Init(int width, int height, int maxLayers);

    // UploadLayer, upload pixel to a specific layer, return the index of the layer or -1 if fails
    int UploadLayer(const uint8_t* pixels, int width, int height);

    // Reload a layer if you needed, hot-reload. 
    bool UpdateLayer(int layer, const uint8_t* pixels, int width, int height);

    void Bind(uint32_t unit = 0) const;

    void Destroy();

    GLuint GetID()        const { return m_id; }
    int    GetWidth()     const { return m_width; }
    int    GetHeight()    const { return m_height; }
    int    GetMaxLayers() const { return m_maxLayers; }
    int    GetUsedLayers()const { return m_nextLayer; }
    bool   IsValid()      const { return m_id != 0; }

  private:
    GLuint m_id        = 0;
    int    m_width     = 0;
    int    m_height    = 0;
    int    m_maxLayers = 0;
    int    m_nextLayer = 0;
  };
}
