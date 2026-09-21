// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
// | -------------------------------

namespace ENG
{
  /**
   * @brief 2D camera with position, zoom and rotation.
   *
   * Camera2D defines which part of the world is visible. It stores the
   * position of the camera (the center of the view), the size of the
   * viewport, a zoom factor and a rotation, and provides the matrices needed
   * to render the world and to convert screen coordinates to world
   * coordinates.
   *
   * The camera can follow a target (an ITransform or an IBoundingBox) when
   * Update() is called every frame.
   *
   * The world uses screen-like axes: X grows to the right and Y grows
   * downwards.
   *
   * Usage:
   * @code
   * ENG::Camera2D camera(1280.0f, 720.0f);
   * camera.SetTarget(&playerTransform);
   * camera.SetZoom(2.0f);
   * // every frame:
   * camera.Update(dt);
   * glm::mat4 vp = camera.GetViewProjectionMatrix();
   * @endcode
   */
  class Camera2D
  {
    public:
      /**
       * @brief Creates a camera from the size of the viewport.
       *
       * The camera starts at position { 0, 0 }, with zoom 1.0 and no rotation.
       *
       * @param[in] viewportWidth  Width of the viewport, in pixels.
       * @param[in] viewportHeight Height of the viewport, in pixels.
       */
      Camera2D(float viewportWidth, float viewportHeight)
        : m_position(0.0f, 0.0f), m_viewportSize(viewportWidth, viewportHeight), m_zoom(1.0f), m_rotation(0.0f)
      {}

      /**
       * @brief Creates a camera from the size of the viewport.
       *
       * The camera starts at position { 0, 0 }, with zoom 1.0 and no rotation.
       *
       * @param[in] viewportSize Size of the viewport: { width, height }, in pixels.
       */
      Camera2D(const Vector2& viewportSize)
        : m_position(0.0f, 0.0f), m_viewportSize(viewportSize), m_zoom(1.0f), m_rotation(0.0f)
      {}

      /**
       * @brief Updates the camera once per frame.
       *
       * If a target was set, the camera is centered on it. When both an
       * ITransform target and an IBoundingBox target are set, the bounding
       * box takes precedence. The camera follows the target instantly, with
       * no smoothing.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       *               Currently unused.
       */
      void Update(float dt)
      {
        if(m_target != nullptr)
          m_position = m_target->m_position;
        if(m_target_bb != nullptr)
        {
          m_position = m_target_bb->GetCenter();
        }
      }

      /**
       * @brief Sets an ITransform for the camera to follow.
       *
       * The target is not owned by the camera and must outlive it (or be
       * replaced before it is destroyed).
       *
       * @param[in] _target Transform to follow.
       */
      void SetTarget(const ITransform* _target) {m_target = _target;}

      /**
       * @brief Sets an IBoundingBox for the camera to follow.
       *
       * The camera centers on the center of the bounding box. The target is
       * not owned by the camera and must outlive it (or be replaced before
       * it is destroyed).
       *
       * @param[in] _target Bounding box to follow.
       */
      void SetTarget(const IBoundingBox* _target) {m_target_bb = _target;}

      /**
       * @brief Sets the position of the camera.
       *
       * @param[in] position New position: the point of the world at the
       *                     center of the view.
       */
      void SetPosition(const Vector2 &position) { m_position = position;}

      /**
       * @brief Sets the position of the camera.
       *
       * @param[in] x New X coordinate.
       * @param[in] y New Y coordinate.
       */
      void SetPosition(float x, float y) { m_position.x = x; m_position.y = y;}

      /**
       * @brief Returns the position of the camera.
       * @return The point of the world at the center of the view.
       */
      const Vector2& GetPosition() const { return m_position; }

      /**
       * @brief Moves the camera by an offset.
       *
       * @param[in] delta Offset to add to the position of the camera.
       */
      void Move(const Vector2 &delta) { m_position += delta; }

      /**
       * @brief Sets the zoom of the camera.
       *
       * @param[in] zoom New zoom. 1.0 is the normal scale; values above 1.0
       *                 zoom in and values below 1.0 zoom out. It must be
       *                 greater than 0.
       */
      void SetZoom(float zoom) { m_zoom = zoom;}

      /**
       * @brief Returns the zoom of the camera.
       * @return The current zoom. 1.0 is the normal scale.
       */
      float GetZoom() const { return m_zoom; }

      /**
       * @brief Sets the rotation of the camera.
       *
       * @param[in] radians New rotation, in radians.
       */
      void SetRotation(float radians) { m_rotation = radians; }

      /**
       * @brief Returns the rotation of the camera.
       * @return The current rotation, in radians.
       */
      float GetRotation() const { return m_rotation; }

      /**
       * @brief Changes the size of the viewport.
       *
       * @param[in] width  New width, in pixels.
       * @param[in] height New height, in pixels.
       */
      void Resize(float width, float height) { m_viewportSize = {width, height}; }

      /**
       * @brief Changes the size of the viewport.
       *
       * @param[in] size New size: { width, height }, in pixels.
       */
      void Resize(const Vector2& size) { m_viewportSize = size; }

      /**
       * @brief Returns the size of the viewport.
       * @return The size of the viewport: { width, height }, in pixels.
       */
      const Vector2 &GetViewportSize() const { return m_viewportSize; }

      /**
       * @brief Returns the axis-aligned bounding box (AABB) of the visible area.
       *
       * The area is centered on the camera position and takes the zoom into
       * account. The rotation of the camera is not taken into account.
       *
       * @warning The members of the returned Rectangle do not have their
       *          usual meaning. Here `x` and `y` hold the top-left corner,
       *          but `w` and `h` hold the bottom-right corner
       *          (x + width, y + height) instead of the width and the
       *          height.
       *
       * @return The visible area of the world.
       */
      Rectangle GetRectCamera() const
      {
        Vector2 halfExtent = m_viewportSize * 0.5f / m_zoom;
        Rectangle rectCamera = {
          .x = m_position.x - halfExtent.x,
          .y = m_position.y - halfExtent.y,
          .w = m_position.x + halfExtent.x,
          .h = m_position.y + halfExtent.y
        };

        return rectCamera;
      }

      /**
       * @brief Returns the view matrix of the camera.
       *
       * It is the inverse of the camera's transform (position, rotation and
       * zoom), so it moves the world into the camera's point of view.
       *
       * @return The view matrix.
       */
      glm::mat4 GetViewMatrix() const
      {
        glm::vec2 position = {m_position.x,m_position.y};
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        transform = glm::rotate(transform, m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(1.0f/m_zoom,1.0f/ m_zoom, 1.0f));
        return glm::inverse(transform);
      }

      /**
       * @brief Returns the projection matrix of the camera.
       *
       * Orthographic projection centered at the origin, in world units. The
       * Y axis points downwards.
       *
       * @return The projection matrix.
       */
      glm::mat4 GetProjectionMatrix() const
      {
        float halfW = m_viewportSize.x * 0.5f;
        float halfH = m_viewportSize.y * 0.5f;
        return glm::ortho(-halfW, halfW, halfH, -halfH, -1.0f, 1.0f);
      }

      /**
       * @brief Returns the combined view-projection matrix.
       *
       * @return projection * view. It transforms world coordinates to clip
       *         space, ready to be sent to a shader.
       */
      glm::mat4 GetViewProjectionMatrix() const
      {
        return GetProjectionMatrix() * GetViewMatrix();
      }

      /**
       * @brief Converts a screen position to world coordinates.
       *
       * The screen position is given in pixels with the origin at the top-left
       * corner. The viewport size is used as the size of the screen, so it
       * must match the size of the window (or render area).
       *
       * @param[in] screenPos Position on the screen, in pixels.
       * @return The corresponding position in the world.
       */
      Vector2 ScreenToWorld(const Vector2 &screenPos) const
      {
        glm::vec2 ndc = {(screenPos.x / m_viewportSize.x) * 2.0f - 1.0f, 1.0f - (screenPos.y / m_viewportSize.y) * 2.0f};
        glm::mat4 invVP = glm::inverse(GetViewProjectionMatrix());
        glm::vec4 worldPos = invVP * glm::vec4(ndc, 0.0f, 1.0f);
        glm::vec2 wPos = glm::vec2(worldPos);

        return {wPos.x,wPos.y};
      }

    private:
      /// @brief Transform the camera follows. nullptr if none. Not owned.
      const ITransform* m_target = nullptr;

      /// @brief Bounding box the camera follows. nullptr if none. Not owned.
      const IBoundingBox* m_target_bb = nullptr;

      /// @brief Position of the camera: the point of the world at the center of the view.
      Vector2 m_position;

      /// @brief Size of the viewport: { width, height }, in pixels.
      Vector2 m_viewportSize;

      /// @brief Zoom factor. 1.0 is the normal scale.
      float m_zoom;

      /// @brief Rotation of the camera, in radians.
      float m_rotation;
  };
}
