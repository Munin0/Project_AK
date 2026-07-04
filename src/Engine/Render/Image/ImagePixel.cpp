// | -------------------------------
#include "ImagePixel.hpp"
// | -------------------------------
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  ImagePixels LoadImagePixels(const std::string& path)
  {
    ImagePixels img;
    stbi_set_flip_vertically_on_load(false);

    img.data = stbi_load(path.c_str(), &img.width, &img.height, &img.channels, STBI_rgb_alpha);

    if (!img.data)
    {
      LOG_ERROR("Failed to load image on ImagePixels: " + path + " - " + stbi_failure_reason());
    }

    return img;
  }
}

