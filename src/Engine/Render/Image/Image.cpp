// | -------------------------------
#include "Image.hpp"
// | -------------------------------
#include <glad/glad.h>
// | -------------------------------
#include "Engine/Utils/Log.hpp"
#include "Engine/Extern/stb_image.h"
// | -------------------------------
#include <cstdint>
#include <stdexcept>
#include <string>
// | -------------------------------

namespace ENG
{
  RImage::RImage(const std::string& path, const std::string& name)
    : m_path(path), m_name(name)
  {}

  RImage::~RImage()
  {
    Free();
  }
  
  RImage::RImage(RImage&& other) noexcept
    :m_id{other.m_id}, m_width(other.m_width), m_height(other.m_height)
  {
    other.m_id = 0 ;
  }

  RImage& RImage::operator=(RImage&& other) noexcept
  {
    if(this != &other)
    {
      Free();
      m_id        = other.m_id;
      m_width     = other.m_width;
      m_height    = other.m_height;
      other.m_id  = 0;
    }
    return *this;
  }

  void RImage::LoadImage()
  {
    Free();

    // stb_image load to RAM
    int channels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* pixels = stbi_load(this->m_path.c_str(), &m_width, &m_height, &channels, 0);
    if(!pixels)
    {
      throw std::runtime_error("RImage: can't load this: " + this->m_path);
    }
    else
    {
      LOG_INFO(" | << Image loaded successfully");
      LOG_INFO(" | <<    ->"+m_path);
    }

    
    GLenum format = (channels == 4) ? GL_RGBA : (channels == 1 ? GL_RED : GL_RGB);
    LOG_INFO(" | << Channels: " + std::to_string(channels));

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load to VRAM
    glGenTextures(1,&m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // SAMPLING
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 
    if (channels == 1)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format,
          m_width, m_height, 0,
          format, GL_UNSIGNED_BYTE, pixels
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    stbi_image_free(pixels);
    glBindTexture(GL_TEXTURE_2D,0);
  }

  void RImage::Free()
  {
    if(m_id)
    {
      glDeleteTextures(1,&m_id);
      m_id = 0;
    }
  }

  void RImage::Bind(uint32_t slot) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
  }
}
