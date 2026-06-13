// | -------------------------------
#include "RImage.hpp"
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
    : _path(path), _name(name)
  {}

  RImage::~RImage()
  {
    Free();
  }
  
  RImage::RImage(RImage&& other) noexcept
    :m_ID{other.m_ID}, m_Width(other.m_Width), m_Height(other.m_Height)
  {
    other.m_ID = 0 ;
  }

  RImage& RImage::operator=(RImage&& other) noexcept
  {
    if(this != &other)
    {
      Free();
      m_ID        = other.m_ID;
      m_Width     = other.m_Width;
      m_Height    = other.m_Height;
      other.m_ID  = 0;
    }
    return *this;
  }

  void RImage::LoadImage()
  {
    Free();

    // stb_image load to RAM
    int channels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* pixels = stbi_load(this->_path.c_str(), &m_Width, &m_Height, &channels, 0);
    if(!pixels)
    {
      throw std::runtime_error("RImage: can't load this: " + this->_path);
    }
    else
    {
      LOG_INFO(" | << Image loaded successfully");
      LOG_INFO(" | <<    ->"+_path);
    }

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    LOG_INFO(" | << Channels: " + std::to_string(channels));
    // Load to VRAM
    glGenTextures(1,&m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    // SAMPLING
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, format,
          m_Width, m_Height, 0,
          format, GL_UNSIGNED_BYTE, pixels
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);
    glBindTexture(GL_TEXTURE_2D,0);
  }

  void RImage::Free()
  {
    if(m_ID)
    {
      glDeleteTextures(1,&m_ID);
      m_ID = 0;
    }
  }

  void RImage::Bind(uint32_t slot) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
  }
}
