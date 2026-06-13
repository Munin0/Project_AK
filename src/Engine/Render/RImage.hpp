// | -------------------------------
#pragma once
// | -------------------------------
#include <glad/glad.h>
// | -------------------------------
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <cstdint>
#include <string>
// | -------------------------------

namespace ENG
{
  class RImage
  {
    public:
      RImage() = default;
      RImage(const std::string& path, const std::string& name);
      ~RImage();

      // No Copies, on GL we dont copy the texture.
      RImage(const RImage&) = delete;
      RImage& operator=(const RImage&) = delete;

      // Move is a good practice.
      RImage(RImage&& other) noexcept;
      RImage& operator=(RImage&& other) noexcept;

      void LoadImage(void);
      Vector2 GetSize(void) {return {(float)m_Width,(float)m_Height};}
      
      void Bind(uint32_t slot = 0) const;

      GLuint      GetID()      const {return m_ID;}
      std::string GetName()    const {return _name;}
      int         GetWidth()   const {return m_Width;}
      int         GetHeight()  const {return m_Height;}
      bool        IsValid()    const {return m_ID != 0;}
    private:
      void Free();
    public:
    private:
      std::string _path = "";
      std::string _name = "";
      GLuint m_ID   = 0;
      int m_Width   = 0;
      int m_Height  = 0;
  };

}
