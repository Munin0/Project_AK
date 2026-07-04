// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Extern/stb_image.h"
// | -------------------------------
#include <cstdint>
#include <string>
// | -------------------------------

namespace ENG
{
  struct ImagePixels
  {
    uint8_t* data = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;

    ~ImagePixels()
    {
      if (data) stbi_image_free(data);
    }

    // no copiable, evita doble-free del buffer de stb_image
    ImagePixels(const ImagePixels&) = delete;
    ImagePixels& operator=(const ImagePixels&) = delete;

    ImagePixels() = default;
    ImagePixels(ImagePixels&& other) noexcept
      : data(other.data), width(other.width), height(other.height), channels(other.channels)
    {
      other.data = nullptr;
    }
  };
  [[maybe_unused]]ImagePixels LoadImagePixels(const std::string& path);
}
