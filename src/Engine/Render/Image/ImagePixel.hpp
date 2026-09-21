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
  /**
   * @brief Pixel data of an image loaded from disk.
   *
   * ImagePixels owns the pixel buffer returned by stb_image and frees it
   * automatically (RAII) when the object is destroyed.
   *
   * The buffer is tightly packed, with `width * height * channels` bytes.
   *
   * @note It is not copyable, to avoid a double free of the stb_image
   *       buffer. It can be moved.
   */
  struct ImagePixels
  {
    /// @brief Pixel buffer, allocated by stb_image. nullptr if no image is loaded.
    uint8_t* data = nullptr;
    /// @brief Width of the image, in pixels.
    int width = 0;
    /// @brief Height of the image, in pixels.
    int height = 0;
    /// @brief Number of color channels per pixel (for example, 4 for RGBA).
    int channels = 0;
    /**
     * @brief Frees the pixel buffer with stbi_image_free().
     */
    ~ImagePixels()
    {
      if (data) stbi_image_free(data);
    }
    /// @brief Not copyable, to avoid a double free of the stb_image buffer.
    ImagePixels(const ImagePixels&) = delete;
    /// @brief Not copy-assignable, to avoid a double free of the stb_image buffer.
    ImagePixels& operator=(const ImagePixels&) = delete;
    /// @brief Creates an empty ImagePixels, with no image loaded.
    ImagePixels() = default;
    /**
     * @brief Move constructor.
     *
     * Takes ownership of the pixel buffer of @p other, which is left
     * without a buffer (its data is set to nullptr).
     *
     * @param[in,out] other ImagePixels to move from.
     */
    ImagePixels(ImagePixels&& other) noexcept
      : data(other.data), width(other.width), height(other.height), channels(other.channels)
    {
      other.data = nullptr;
    }
  };

  /**
   * @brief Loads an image file into memory.
   *
   * Decodes the image with stb_image and returns its pixels together with
   * its size and number of channels.
   *
   * @param[in] path Path of the image file to load.
   * @return The loaded image. If the image could not be loaded, the
   *         returned ImagePixels has a nullptr data.
   */
  [[maybe_unused]]ImagePixels LoadImagePixels(const std::string& path);
}
