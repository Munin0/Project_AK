/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include <glad/glad.h>
/// | ------------------------------------ |
#include <cstdint>
/// | ------------------------------------ |

namespace ENG
{
  /**
   * @brief OpenGL 2D texture array that stores many images as layers.
   *
   * TextureArray reserves space on the GPU for a fixed number of layers of
   * the same size and lets images (for example, atlases) be uploaded into
   * them one by one. A shader can then pick any layer with a single
   * sampler, which allows drawing sprites from different atlases in the
   * same batch (see Vertex::ArrayLayer).
   *
   * Usage:
   * @code
   * ENG::TextureArray array;
   * array.Init(2048, 2048, 8);
   * int layer = array.UploadLayer(pixels, 512, 512);
   * array.Bind(0);
   * @endcode
   *
   * @note An OpenGL context must be current when using it, including when
   *       it is destroyed.
   */
  class TextureArray
  {
  public:
    /// @brief Creates an empty TextureArray, with no GPU storage.
    TextureArray() = default;

    /**
     * @brief Destroys the TextureArray and its OpenGL texture.
     */
    ~TextureArray();

    /// @brief Not copyable: the GPU texture has a single owner.
    TextureArray(const TextureArray&) = delete;

    /// @brief Not copy-assignable: the GPU texture has a single owner.
    TextureArray& operator=(const TextureArray&) = delete;

    /**
     * @brief Move constructor.
     *
     * Takes ownership of the texture of @p other, which is left empty.
     *
     * @param[in,out] other TextureArray to move from.
     */
    TextureArray(TextureArray&& other) noexcept;

    /**
     * @brief Move assignment.
     *
     * Takes ownership of the texture of @p other, which is left empty.
     *
     * @param[in,out] other TextureArray to move from.
     * @return Reference to this TextureArray.
     */
    TextureArray& operator=(TextureArray&& other) noexcept;

    /**
     * @brief Reserves space on the GPU for all the layers.
     *
     * The memory of every layer is reserved at once, so the size of the
     * array is fixed after this call.
     *
     * @param[in] width     Width of each layer, in pixels.
     * @param[in] height    Height of each layer, in pixels.
     * @param[in] maxLayers Maximum number of layers of the array.
     * @return true if the array was created successfully, false otherwise.
     */
    bool Init(int width, int height, int maxLayers);

    /**
     * @brief Uploads pixel data to the next free layer.
     *
     * @param[in] pixels Pixel data of the image.
     * @param[in] width  Width of the image, in pixels.
     * @param[in] height Height of the image, in pixels.
     * @return The index of the layer where the image was stored, or -1 if
     *         the upload failed.
     */
    int UploadLayer(const uint8_t* pixels, int width, int height);

    /**
     * @brief Replaces the pixel data of a layer that was already uploaded.
     *
     * Useful to reload an image while the game is running (hot-reload).
     *
     * @param[in] layer  Index of the layer to update.
     * @param[in] pixels New pixel data of the image.
     * @param[in] width  Width of the image, in pixels.
     * @param[in] height Height of the image, in pixels.
     * @return true if the layer was updated successfully, false otherwise.
     */
    bool UpdateLayer(int layer, const uint8_t* pixels, int width, int height);

    /**
     * @brief Binds the texture array to a texture unit.
     *
     * @param[in] unit Texture unit to bind to. Defaults to 0.
     */
    void Bind(uint32_t unit = 0) const;

    /**
     * @brief Destroys the OpenGL texture and resets the TextureArray.
     */
    void Destroy();

    /**
     * @brief Returns the OpenGL texture ID.
     * @return The texture ID, or 0 if the array has not been created.
     */
    GLuint GetID()        const { return m_id; }

    /**
     * @brief Returns the width of each layer.
     * @return Width, in pixels.
     */
    int    GetWidth()     const { return m_width; }

    /**
     * @brief Returns the height of each layer.
     * @return Height, in pixels.
     */
    int    GetHeight()    const { return m_height; }

    /**
     * @brief Returns the maximum number of layers.
     * @return The capacity of the array, in layers.
     */
    int    GetMaxLayers() const { return m_maxLayers; }

    /**
     * @brief Returns the number of layers already in use.
     * @return The number of layers that have been uploaded.
     */
    int    GetUsedLayers()const { return m_nextLayer; }

    /**
     * @brief Tells whether the texture array exists on the GPU.
     * @return true if the OpenGL texture exists (its ID is not 0), false otherwise.
     */
    bool   IsValid()      const { return m_id != 0; }

  private:
    /// @brief OpenGL texture ID. 0 if there is no texture.
    GLuint m_id        = 0;

    /// @brief Width of each layer, in pixels.
    int    m_width     = 0;

    /// @brief Height of each layer, in pixels.
    int    m_height    = 0;

    /// @brief Maximum number of layers.
    int    m_maxLayers = 0;

    /// @brief Index of the next free layer (that is, the number of layers in use).
    int    m_nextLayer = 0;
  };
}
