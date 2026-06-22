// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Utils/Rects.hpp"
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
  class Drawer
  {
    public:
      static void DrawLine(const Vector2& pInit, const Vector2& pEnd, const Color& _color, float thickness = 1.0f);
      static void DrawTexture(const Vector2& _position, const Vector2& _size, std::shared_ptr<RImage> _texture);
      static void DrawTexture(const Rect& _rectangle, std::shared_ptr<RImage> _texture);
      static void DrawRectangle(const Vector2& _position, const Vector2& _size, const Color& _color);
      static void DrawRectangle(const Rect& _rectangle, const Color& _color);
      static void DrawRectangleOutline(const Rect& _rectangle, const Color& _color, float thickness = 1.0f);
      static void DrawRectangleOutline(const Vector2& _position, const Vector2& _size, const Color& _color, float thickness = 1.0f);
      static void DrawCircle(const Vector2& _center, float _radius, const Color& _color, uint32_t _segments);
      static void DrawCircleOutLine(const Vector2& _center, float _radius, const Color& _color, uint32_t _segments, float thickness = 1.0f);
      static void DrawTriangle(const Vector2& pointA, const Vector2& pointB, const Vector2& pointC, const Color& _color);
      static void DrawTriangle(const std::vector<Vector2>& _points, const Color& _color);
      static void DrawTriangleOutline(const Vector2& pointA, const Vector2& pointB, const Vector2& pointC, const Color& _color, float thickness = 1.0f);
      static void DrawPolygon(const std::vector<Vector2>& _points, const Color& _color);
      static void DrawPolygonOutline(const std::vector<Vector2>& _points, const Color& _color, float thickness = 1.0f);
  };
}
