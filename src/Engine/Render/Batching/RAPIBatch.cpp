#include "RAPIBatch.hpp"
// | -------------------------------
#include "Engine/Render/Render.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Rects.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Image/RImage.hpp"
// | -------------------------------
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
// | -------------------------------
#include <memory>
#include <vector>
#include <cstdint>
// | -------------------------------

namespace ENG
{
  void Drawer::DrawLine(const Vector2& pInit, const Vector2& pEnd, const Color& _color, float thickness)
  {
    glm::vec2 posA = {pInit.x,pInit.y};
    glm::vec2 posB = {pEnd.x, pEnd.y};
    glm::vec4 color = {_color.r,_color.g,_color.b,_color.a};
    Render::Get().GetBatcher().DrawLine(posA, posB, thickness, color);
  }

  void Drawer::DrawTexture(const Vector2& _position, const Vector2& _size, std::shared_ptr<RImage> _texture)
  {
    glm::vec2 pos = {_position.x, _position.y};
    glm::vec2 size = {_size.x, _size.y};
    Render::Get().GetBatcher().DrawTexture(pos, size, _texture);
  }

  void Drawer::DrawTexture(const Rect& _rectangle, std::shared_ptr<RImage> _texture)
  {
    glm::vec2 pos = {_rectangle.x, _rectangle.y};
    glm::vec2 size = {_rectangle.w, _rectangle.h};
    Render::Get().GetBatcher().DrawTexture(pos, size, _texture);
  }

  void Drawer::DrawRectangle(const Vector2& _position, const Vector2& _size, const Color& _color)
  {
    glm::vec2 pos = {_position.x, _position.y};
    glm::vec2 size = {_size.x, _size.y};
    glm::vec4 color = {_color.r,_color.g,_color.b,_color.a};
    Render::Get().GetBatcher().DrawQuad(pos, size, color);
  }

  void Drawer::DrawRectangle(const Rect& _rectangle, const Color& _color)
  {
    glm::vec2 pos = {_rectangle.x, _rectangle.y};
    glm::vec2 size = {_rectangle.w, _rectangle.h};
    glm::vec4 color = {_color.r,_color.g,_color.b,_color.a};
    Render::Get().GetBatcher().DrawQuad(pos, size, color);
  }

  void Drawer::DrawRectangleOutline(const Vector2& _position, const Vector2& _size, const Color& _color, float thickness)
  {
    glm::vec2 pos = {_position.x, _position.y};
    glm::vec2 size = {_size.x, _size.y};
    Render::Get().GetBatcher().DrawQuadOutline(pos.x,pos.y, size.x,size.y, _color,thickness);
  }

  void Drawer::DrawRectangleOutline(const Rect& _rectangle, const Color& _color, float thickness)
  {
    glm::vec2 pos = {_rectangle.x, _rectangle.y};
    glm::vec2 size = {_rectangle.w, _rectangle.h};
    glm::vec4 color = {_color.r,_color.g,_color.b,_color.a};
    Render::Get().GetBatcher().DrawQuadOutline(pos.x,pos.y, size.x,size.y, _color,thickness);
  }

  void Drawer::DrawCircle(const Vector2& _center, float _radius, const Color& _color, uint32_t _segments)
  {
    glm::vec2 pos = {_center.x,_center.y};
    glm::vec4 color = {_color.r,_color.g,_color.b,_color.a};
    Render::Get().GetBatcher().DrawCircle(pos, _radius, color);
  }

  void Drawer::DrawCircleOutLine(const Vector2& _center, float _radius, const Color& _color, uint32_t _segments, float thickness)
  {
    glm::vec2 pos = {_center.x,_center.y};
    glm::vec4 color = {_color.r,_color.g,_color.b,_color.a};
    Render::Get().GetBatcher().DrawCircleOutLine(pos, _radius, color, thickness);
  }

  void Drawer::DrawTriangle(const Vector2& pointA, const Vector2& pointB, const Vector2& pointC, const Color& _color)
  {
    glm::vec2 pA = {pointA.x, pointA.y};
    glm::vec2 pB = {pointB.x, pointB.y};
    glm::vec2 pC = {pointC.x, pointC.y};
    glm::vec4 color = {_color.r,_color.g,_color.b,_color.a};
    Render::Get().GetBatcher().DrawTriangle(pA, pB, pC, color);
  }

  void Drawer::DrawTriangle(const std::vector<Vector2>& _points, const Color& _color)
  {
    if(_points.size() != 3)
    {
      LOG_ERROR(" | << Size of the Vector is different of 3, need only 3 points for DrawTriangle");
      return;
    }
    glm::vec2 pA = {_points[0].x, _points[0].y};
    glm::vec2 pB = {_points[1].x, _points[1].y};
    glm::vec2 pC = {_points[2].x, _points[2].y};
    glm::vec4 color = {_color.r,_color.g,_color.b,_color.a};
    Render::Get().GetBatcher().DrawTriangle(pA, pB, pC, color);
  }

  void Drawer::DrawTriangleOutline(const Vector2& pointA, const Vector2& pointB, const Vector2& pointC, const Color& _color, float thickness)
  {
    auto b = Render::Get().GetBatcher();
    glm::vec2 pA = {pointA.x, pointA.y};
    glm::vec2 pB = {pointB.x, pointB.y};
    glm::vec2 pC = {pointC.x, pointC.y};
    glm::vec4 color = {_color.r,_color.g,_color.b,_color.a};
    
    b.DrawLine( pA, pB,thickness, color);
    b.DrawLine( pB, pC,thickness, color);
    b.DrawLine( pC, pA,thickness, color);
  }

  void Drawer::DrawPolygon(const std::vector<Vector2>& _points, const Color& _color)
  {
    if(_points.size() < 3)
    {
      LOG_ERROR(" | << Polygon needs >3 points to be drawed");
      return;
    }
    std::vector<glm::vec2> points;
    glm::vec4 color = {_color.r,_color.g,_color.b,_color.a};
    for(auto& p : _points)
      points.push_back({p.x,p.y});
    Render::Get().GetBatcher().DrawPolygon(points,color);
  }

  void Drawer::DrawPolygonOutline(const std::vector<Vector2>& _points, const Color& _color, float thickness)
  {
    auto b = Render::Get().GetBatcher();
    std::vector<glm::vec2> points;
    glm::vec4 color = {_color.r,_color.g,_color.b,_color.a};
    for(auto& p : _points)
      points.push_back({p.x,p.y});
    points.push_back(points[0]);
    for(int i=0; i<points.size()-1; i++)
    {
      glm::vec2 point = {points[i].x, points[i].y};
      glm::vec2 pointNext = {points[i+1].x, points[i+1].y};
      b.DrawLine(point,pointNext, thickness, color);
    }  
  }
}

