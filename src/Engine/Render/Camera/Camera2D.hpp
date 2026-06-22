// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
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
  class Camera2D
  {
    public:
      Camera2D(float viewportWidth, float viewportHeight)
        : m_position(0.0f, 0.0f), m_viewportSize(viewportWidth, viewportHeight), m_zoom(1.0f), m_rotation(0.0f)
      {}
      Camera2D(const Vector2& viewportSize)
        : m_position(0.0f, 0.0f), m_viewportSize(viewportSize), m_zoom(1.0f), m_rotation(0.0f)
      {}

      void Update(float dt)
      {
        if(m_target != nullptr)
          m_position = m_target->position;
        if(m_target_bb != nullptr)
        {
          m_position = m_target_bb->GetPosition() + m_target_bb->GetCenter();
        }
      }

      void SetTarget(const ITransform* _target) {m_target = _target;}
      void SetTarget(const IBoundingBox* _target) {m_target_bb = _target;}
      void SetPosition(const Vector2 &position) { m_position = position;}
      void SetPosition(float x, float y) { m_position.x = x; m_position.y = y;}
      const Vector2& GetPosition() const { return m_position; }
      void Move(const Vector2 &delta) { m_position += delta; }
      void SetZoom(float zoom) { m_zoom = zoom;}
      float GetZoom() const { return m_zoom; }
      void SetRotation(float radians) { m_rotation = radians; }
      float GetRotation() const { return m_rotation; }
      void Resize(float width, float height) { m_viewportSize = {width, height}; }
      void Resize(const Vector2& size) { m_viewportSize = size; }
      const Vector2 &GetViewportSize() const { return m_viewportSize; }

      // Matriz de vista: inversa de la transformación de la cámara.
      glm::mat4 GetViewMatrix() const 
      {
        glm::vec2 position = {m_position.x,m_position.y};
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        transform = glm::rotate(transform, m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(1.0f/m_zoom,1.0f/ m_zoom, 1.0f));
        return glm::inverse(transform);
      }

      // Proyección ortográfica centrada en el origen, en unidades de mundo.
      glm::mat4 GetProjectionMatrix() const
      {
        float halfW = m_viewportSize.x * 0.5f;
        float halfH = m_viewportSize.y * 0.5f;
        return glm::ortho(-halfW, halfW, halfH, -halfH, -1.0f, 1.0f);
      }

      glm::mat4 GetViewProjectionMatrix() const
      {
        return GetProjectionMatrix() * GetViewMatrix();
      }

      Vector2 ScreenToWorld(const Vector2 &screenPos) const 
      {
        glm::vec2 ndc = {(screenPos.x / m_viewportSize.x) * 2.0f - 1.0f, 1.0f - (screenPos.y / m_viewportSize.y) * 2.0f};
        glm::mat4 invVP = glm::inverse(GetViewProjectionMatrix());
        glm::vec4 worldPos = invVP * glm::vec4(ndc, 0.0f, 1.0f);
        glm::vec2 wPos = glm::vec2(worldPos);

        return {wPos.x,wPos.y};
      }

    private:
      const ITransform* m_target = nullptr;
      const IBoundingBox* m_target_bb = nullptr;
      Vector2 m_position;
      Vector2 m_viewportSize;
      float m_zoom;
      float m_rotation;
  };
}
