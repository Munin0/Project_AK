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
  /**
   * @brief OpenGL 2D texture created from an image file.
   *
   * RImage represents an image that lives on the GPU. It stores the path and
   * the name of the image, and owns the OpenGL texture created from it: the
   * texture is deleted when the RImage is destroyed.
   *
   * Usage:
   * @code
   * ENG::RImage image("player.png", "player");
   * image.LoadImage();
   * if (image.IsValid())
   *   image.Bind(0);
   * @endcode
   *
   * @note An OpenGL context must be current when the texture is created
   *       and when it is destroyed.
   */
  class RImage
  {
    public:
      /// @brief Creates an empty RImage, with no texture.
      RImage() = default;
      /**
       * @brief Creates an RImage for an image file.
       *
       * @param[in] path Path of the image file.
       * @param[in] name Name of the image.
       */
      RImage(const std::string& path, const std::string& name);
      /**
       * @brief Destroys the RImage and its OpenGL texture.
       */
      ~RImage();
      /// @brief Not copyable: a texture on the GPU is not copied, and two
      ///        objects would end up deleting the same texture.
      RImage(const RImage&) = delete;
      /// @brief Not copy-assignable, for the same reason as the copy constructor.
      RImage& operator=(const RImage&) = delete;
      /**
       * @brief Move constructor.
       *
       * Takes ownership of the texture of @p other, which is left empty.
       *
       * @param[in,out] other RImage to move from.
       */
      RImage(RImage&& other) noexcept;
      /**
       * @brief Move assignment.
       *
       * Takes ownership of the texture of @p other, which is left empty.
       *
       * @param[in,out] other RImage to move from.
       * @return Reference to this RImage.
       */
      RImage& operator=(RImage&& other) noexcept;
      /**
       * @brief Loads the image file and creates the OpenGL texture.
       *
       * Reads the image at the path given at construction, uploads it to
       * the GPU and stores the texture ID and the image size. After a
       * successful load IsValid() returns true.
       */
      void LoadImage(void);
      /**
       * @brief Returns the size of the image.
       *
       * @return A Vector2 where x is the width and y is the height, in pixels.
       */
      Vector2 GetSize(void) {return {(float)m_Width,(float)m_Height};}
      /**
       * @brief Binds the texture to a texture unit.
       *
       * @param[in] slot Texture unit to bind to. Defaults to 0.
       */
      void Bind(uint32_t slot = 0) const;
      /**
       * @brief Returns the OpenGL texture ID.
       * @return The texture ID, or 0 if no texture has been created.
       */
      GLuint      GetID()      const {return m_ID;}
      /**
       * @brief Returns the name of the image.
       * @return The name given at construction.
       */
      std::string GetName()    const {return _name;}
      /**
       * @brief Returns the width of the image.
       * @return Width, in pixels.
       */
      int         GetWidth()   const {return m_Width;}
      /**
       * @brief Returns the height of the image.
       * @return Height, in pixels.
       */
      int         GetHeight()  const {return m_Height;}
      /**
       * @brief Tells whether the RImage holds a texture.
       * @return true if the OpenGL texture exists (its ID is not 0), false otherwise.
       */
      bool        IsValid()    const {return m_ID != 0;}
    private:
      /**
       * @brief Deletes the OpenGL texture, if there is one.
       */
      void Free();
    public:
    private:
      /// @brief Path of the image file.
      std::string _path = "";
      /// @brief Name of the image.
      std::string _name = "";
      /// @brief OpenGL texture ID. 0 if there is no texture.
      GLuint m_ID   = 0;
      /// @brief Width of the image, in pixels.
      int m_Width   = 0;
      /// @brief Height of the image, in pixels.
      int m_Height  = 0;
  };

}
