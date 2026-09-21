// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Image/RImage.hpp"
// | -------------------------------
#include <memory>
#include <vector>
#include <cstdint>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Immediate drawing helpers for basic shapes and textures.
   *
   * Drawer groups static functions to draw lines, rectangles, circles,
   * triangles, polygons and textures directly, without creating an Object.
   * It is meant to be used while a frame is being drawn.
   *
   * Usage:
   * @code
   * ENG::Drawer::DrawRectangle({100.0f, 100.0f}, {50.0f, 30.0f}, ENG::Color::Red);
   * ENG::Drawer::DrawCircle({200.0f, 150.0f}, 25.0f, ENG::Color::Blue, 32);
   * @endcode
   *
   * @note Drawer only has static functions and is not meant to be instantiated.
   */
  class Drawer
  {
    public:
      /**
       * @brief Draws a line between two points.
       *
       * @param[in] pInit     Start point.
       * @param[in] pEnd      End point.
       * @param[in] _color    Color of the line.
       * @param[in] thickness Thickness of the line. Defaults to 1.0.
       */
      static void DrawLine(const Vector2& pInit, const Vector2& pEnd, const Color& _color, float thickness = 1.0f);

      /**
       * @brief Draws a texture at a position with a given size.
       *
       * @param[in] _position Position of the texture.
       * @param[in] _size     Size to draw the texture with: { width, height }.
       * @param[in] _texture  Texture to draw.
       */
      static void DrawTexture(const Vector2& _position, const Vector2& _size, std::shared_ptr<RImage> _texture);

      /**
       * @brief Draws a texture inside a rectangle.
       *
       * @param[in] _rectangle Area to draw the texture in: position { x, y }
       *                       and size { w, h }.
       * @param[in] _texture   Texture to draw.
       */
      static void DrawTexture(const Rectangle& _rectangle, std::shared_ptr<RImage> _texture);

      /**
       * @brief Draws a filled rectangle.
       *
       * @param[in] _position Position of the rectangle.
       * @param[in] _size     Size of the rectangle: { width, height }.
       * @param[in] _color    Fill color.
       */
      static void DrawRectangle(const Vector2& _position, const Vector2& _size, const Color& _color);

      /**
       * @brief Draws a filled rectangle.
       *
       * @param[in] _rectangle Rectangle to draw: position { x, y } and size { w, h }.
       * @param[in] _color     Fill color.
       */
      static void DrawRectangle(const Rectangle& _rectangle, const Color& _color);

      /**
       * @brief Draws the outline of a rectangle.
       *
       * @param[in] _rectangle Rectangle to draw: position { x, y } and size { w, h }.
       * @param[in] _color     Color of the outline.
       * @param[in] thickness  Thickness of the outline. Defaults to 1.0.
       */
      static void DrawRectangleOutline(const Rectangle& _rectangle, const Color& _color, float thickness = 1.0f);

      /**
       * @brief Draws the outline of a rectangle.
       *
       * @param[in] _position Position of the rectangle.
       * @param[in] _size     Size of the rectangle: { width, height }.
       * @param[in] _color    Color of the outline.
       * @param[in] thickness Thickness of the outline. Defaults to 1.0.
       */
      static void DrawRectangleOutline(const Vector2& _position, const Vector2& _size, const Color& _color, float thickness = 1.0f);

      /**
       * @brief Draws a filled circle.
       *
       * The circle is approximated with a number of segments: more segments
       * give a smoother circle.
       *
       * @param[in] _center   Center of the circle.
       * @param[in] _radius   Radius of the circle.
       * @param[in] _color    Fill color.
       * @param[in] _segments Number of segments used to approximate the circle.
       */
      static void DrawCircle(const Vector2& _center, float _radius, const Color& _color, uint32_t _segments);

      /**
       * @brief Draws the outline of a circle.
       *
       * The circle is approximated with a number of segments: more segments
       * give a smoother circle.
       *
       * @param[in] _center   Center of the circle.
       * @param[in] _radius   Radius of the circle.
       * @param[in] _color    Color of the outline.
       * @param[in] _segments Number of segments used to approximate the circle.
       * @param[in] thickness Thickness of the outline. Defaults to 1.0.
       */
      static void DrawCircleOutLine(const Vector2& _center, float _radius, const Color& _color, uint32_t _segments, float thickness = 1.0f);

      /**
       * @brief Draws a filled triangle from its three vertices.
       *
       * @param[in] pointA First vertex.
       * @param[in] pointB Second vertex.
       * @param[in] pointC Third vertex.
       * @param[in] _color Fill color.
       */
      static void DrawTriangle(const Vector2& pointA, const Vector2& pointB, const Vector2& pointC, const Color& _color);

      /**
       * @brief Draws a filled triangle from a list of vertices.
       *
       * @param[in] _points Vertices of the triangle. Three points are expected.
       * @param[in] _color  Fill color.
       */
      static void DrawTriangle(const std::vector<Vector2>& _points, const Color& _color);

      /**
       * @brief Draws the outline of a triangle.
       *
       * @param[in] pointA    First vertex.
       * @param[in] pointB    Second vertex.
       * @param[in] pointC    Third vertex.
       * @param[in] _color    Color of the outline.
       * @param[in] thickness Thickness of the outline. Defaults to 1.0.
       */
      static void DrawTriangleOutline(const Vector2& pointA, const Vector2& pointB, const Vector2& pointC, const Color& _color, float thickness = 1.0f);

      /**
       * @brief Draws a filled polygon.
       *
       * @param[in] _points Vertices of the polygon, in order.
       * @param[in] _color  Fill color.
       */
      static void DrawPolygon(const std::vector<Vector2>& _points, const Color& _color);

      /**
       * @brief Draws the outline of a polygon.
       *
       * @param[in] _points   Vertices of the polygon, in order.
       * @param[in] _color    Color of the outline.
       * @param[in] thickness Thickness of the outline. Defaults to 1.0.
       */
      static void DrawPolygonOutline(const std::vector<Vector2>& _points, const Color& _color, float thickness = 1.0f);
  };
}
