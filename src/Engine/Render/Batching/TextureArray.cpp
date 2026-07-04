/// | ------------------------------------ |
#include "TextureArray.hpp"
#include "Engine/Utils/Log.hpp"
/// | ------------------------------------ |
#include <cstdint>
#include <string>
/// | ------------------------------------ |

namespace ENG
{
  TextureArray::~TextureArray()
  {
    Destroy();
  }

  TextureArray::TextureArray(TextureArray&& other) noexcept
    : m_id(other.m_id), m_width(other.m_width), m_height(other.m_height), m_maxLayers(other.m_maxLayers), m_nextLayer(other.m_nextLayer)
  {
    other.m_id = 0;
  }

  TextureArray& TextureArray::operator=(TextureArray&& other) noexcept
  {
    if (this != &other)
    {
      Destroy();

      m_id        = other.m_id;
      m_width     = other.m_width;
      m_height    = other.m_height;
      m_maxLayers = other.m_maxLayers;
      m_nextLayer = other.m_nextLayer;

      other.m_id = 0;
    }
    return *this;
  }

  bool TextureArray::Init(int width, int height, int maxLayers)
  {
    if (m_id != 0)
    {
      LOG_ERROR("TextureArray already created, will be destroy before create a new one");
      Destroy();
    }

    m_width     = width;
    m_height    = height;
    m_maxLayers = maxLayers;
    m_nextLayer = 0;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);

    // GL 3.3 core: reserve mutable with glTexImage3D (glTexStorage3D requiere 4.2+)
    glTexImage3D(
      GL_TEXTURE_2D_ARRAY,
      0,                         // mip level
      GL_RGBA8,         // format
      m_width,
      m_height,
      m_maxLayers,               // depth = num of layer 
      0,                        // border, alwats 0
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      nullptr                   // empty, only reserve 
    );

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
      LOG_ERROR("TextureArray::Init error, glError: " + std::to_string(err));
      Destroy();
      return false;
    }

    return true;
  }

  int TextureArray::UploadLayer(const uint8_t* pixels, int width, int height)
  {
    if (!IsValid())
    {
      LOG_ERROR("TextureArray::UploadLayer called without a init");
      return -1;
    }

    if (width > m_width || height > m_height)
    {
      LOG_ERROR("TextureArray::UploadLayer image is bigger than the array's layer size");
      return -1;
    }

    if (m_nextLayer >= m_maxLayers)
    {
      LOG_ERROR("TextureArray::UploadLayer no more layers free, upgrade the space of layers");
      return -1;
    }

    int layer = m_nextLayer;

    glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
    glTexSubImage3D(
      GL_TEXTURE_2D_ARRAY,
      0,                                   // mip level
      0, 0, layer,     // xoffset, yoffset, zoffset (Layer origin)
      width, height, 1,                    // ++ one layer 
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      pixels
    );
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    m_nextLayer++;
    return layer;
  }

  bool TextureArray::UpdateLayer(int layer, const uint8_t* pixels, int width, int height)
  {
    if (!IsValid() || layer < 0 || layer >= m_nextLayer)
    {
      LOG_ERROR("TextureArray::UpdateLayer invalid index");
      return false;
    }

    if (width > m_width || height > m_height)
    {
      LOG_ERROR("TextureArray::UpdateLayer image is bigger than the array's layer size");
      return false;
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
    glTexSubImage3D(
      GL_TEXTURE_2D_ARRAY,
      0,
      0, 0, layer,
      width, height, 1,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      pixels
    );
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    return true;
  }

  void TextureArray::Bind(uint32_t unit) const
  {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
  }

  void TextureArray::Destroy()
  {
    if (m_id != 0)
    {
      glDeleteTextures(1, &m_id);
      m_id = 0;
    }
    m_nextLayer = 0;
  }
}
