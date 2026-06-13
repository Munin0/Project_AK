// | -------------------------------
#include "RGeometry.hpp"
// | -------------------------------
#include "RColor.hpp"
#include "RBatch.hpp"
// | -------------------------------
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Object/Object.hpp"
// | -------------------------------
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
// | -------------------------------
#include <string>
#include <vector>
// | -------------------------------

namespace ENG
{
  inline static int amountGeometries = 0;
  /// Line Part
  Line::Line(float x, float y, float xw, float yh, const Color& c)
    : Object{"Line " + std::to_string(amountGeometries)}, pStart(x, y), pEnd(xw, yh), color(c)
  {
    amountGeometries ++;
    LOG_INFO(" | << Object: Line created, AmountGeometries " + std::to_string(amountGeometries));
  }

  Line::Line(const Vector2& start, const Vector2& end, const Color& c)
    : Object{"Line " + std::to_string(amountGeometries)}, pStart(start), pEnd(end), color(c)
  {
    amountGeometries ++;
    LOG_INFO(" | << Object: Line created, AmountGeometries " + std::to_string(amountGeometries));
  }

  void Line::Draw(Batcher& b) const
  {
    glm::vec2 pointA = {pStart.x, pStart.y};
    glm::vec2 pointB = {pEnd.x, pEnd.y};
    glm::vec4 c = {color.r, color.g, color.b, color.a};

    b.DrawLine(pointA, pointB, 1.0f, c);
  }
  
  void Line::Update(float dt)
  {
    (void)dt;
  }

  /// Rectangle part
  Rectangle::Rectangle(float x, float y, float w, float h, const Color& c)
    : Object("Rectangle " + std::to_string(amountGeometries)),size(w,h),position(x,y),color(c), line_color(c)
  { 
    this->GetTransform().position = {x,y};
    amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(amountGeometries));
  }
  Rectangle::Rectangle(float x, float y, float w, float h, const Color& c, const Color& outColor, float _thickness)
    : Object{"Rectangle " + std::to_string(amountGeometries)}, position(x,y),size(w,h),color(c), line_color(outColor), thicknes{_thickness}
  { 
    amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(amountGeometries));
  }
  Rectangle::Rectangle(float xy, float wh, const Color& c)
    : Object{"Rectangle " + std::to_string(amountGeometries)}, size(wh, wh), position(xy,xy), color(c), line_color(c)
  {
    amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(amountGeometries));
  }

  Rectangle::Rectangle(float xy, float wh, const Color& c, const Color& outColor, float _thickness)
    : Object{"Rectangle " + std::to_string(amountGeometries)}, size(wh,wh), position(xy,xy), color(c), line_color(outColor), thicknes{_thickness}
  {
    amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(amountGeometries));
  }
  Rectangle::Rectangle(const Vector2& position, const Vector2& size, const Color& c)
    : Object{"Rectangle " + std::to_string(amountGeometries)}, size(size), position(position), color{c}, line_color{c} 
  {
    amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(amountGeometries));
  }

  Rectangle::Rectangle(const Vector2& position, const Vector2& size, const Color& c, const Color& outColor, float _thickness)
    : Object{"Rectangle " + std::to_string(amountGeometries)}, size(size), position(position), color{c}, line_color{outColor}, thicknes{_thickness}
  {
    amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(amountGeometries));
  }

  void Rectangle::Draw(Batcher& b) const
  {
    // Fill
    glm::vec4 fillColor = {color.r, color.g, color.b, color.a};
    glm::vec2 pos = {position.x, position.y};
    glm::vec2 size = {this->size.x, this->size.y};

    b.DrawQuad(pos,size,fillColor);

    // Outline
    if(line_color != color)
    {
      float x = position.x, y = position.y;
      float w = size.x, h = size.y;
      
      glm::vec4 outColor = {line_color.r, line_color.g, line_color.b, line_color.a};
      b.DrawQuad({x, y}, {w, thicknes}, outColor);                                                // Top
      b.DrawQuad({x, y + h - thicknes}, {w, thicknes}, outColor);                              // Bottom
      b.DrawQuad({x, y + thicknes}, {thicknes, h - thicknes * 2}, outColor);                   // Left
      b.DrawQuad({x + w - thicknes, y + thicknes}, {thicknes, h - thicknes * 2}, outColor); // Right
    }
  }

  void Rectangle::Update(float dt)
  {
  }

  /// Circle part
  Circle::Circle(float x, float y, float _diameter, const Color& _color)
    : Object{"Circle " + std::to_string(amountGeometries)}, position(x, y), diameter{_diameter}, color{_color}
  {
    amountGeometries ++;
    LOG_INFO(" | << Object: Circle created, AmountGeometries " + std::to_string(amountGeometries));
  }

  Circle::Circle(float x, float y, float _diameter, const Color& _color, const Color& outLine, float _thickness)
    : Object{"Circle " + std::to_string(amountGeometries)}, position{x, y}, diameter{_diameter}, color{_color}, line_color{outLine}, thickness{_thickness}
  {
    amountGeometries ++;
    LOG_INFO(" | << Object: Circle created, AmountGeometries " + std::to_string(amountGeometries));
  }
  Circle::Circle(const Vector2& _position, float _diameter, const Color& _color)
    : Object{"Circle " + std::to_string(amountGeometries)}, position{_position}, diameter{_diameter}, color{_color}
    {
    amountGeometries ++;
    LOG_INFO(" | << Object: Circle created, AmountGeometries " + std::to_string(amountGeometries));
  }
  Circle::Circle(const Vector2& _position, float _diameter, const Color& _color, const Color& outLine, float _thickness)
    : Object{"Circle " + std::to_string(amountGeometries)}, position{_position}, diameter{_diameter}, color{_color}, line_color{outLine}, thickness{_thickness}
  {
    amountGeometries ++;
    LOG_INFO(" | << Object: Circle created, AmountGeometries " + std::to_string(amountGeometries));
  }

  void Circle::Draw(Batcher& b) const
  {
    float rad = diameter * 0.5f;

    glm::vec2 pos= {position.x, position.y};
    glm::vec4 c = {color.r,color.g, color.b, color.a};

    b.DrawCircle(pos, rad, c);

    glm::vec4 oColor = {line_color.r, line_color.g, line_color.b, line_color.a};
    if(line_color != color)
      b.DrawCircleOutLine(pos, rad, oColor, thickness);
  }

  void Circle::Update(float dt)
  {
    (void)dt;
  }

  /// Triangle part
  Triangle::Triangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& _color)
    : Object{"Triangle " + std::to_string(amountGeometries)}, color(_color), outLine{_color}
  {
    points[0] = a;
    points[1] = b;
    points[2] = c;

    amountGeometries ++;
    LOG_INFO(" | << Object: Triangle created, AmountGeometries " + std::to_string(amountGeometries));
  }
  
  Triangle::Triangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& color, const Color& _outLine, float _thickness)
    : Object{"Triangle " + std::to_string(amountGeometries)}, color(color), outLine{_outLine}, thickness{_thickness}
  {
    points[0] = a;
    points[1] = b;
    points[2] = c;

    amountGeometries ++;
    LOG_INFO(" | << Object: Triangle created, AmountGeometries " + std::to_string(amountGeometries));
  }
  void Triangle::Draw(Batcher& b) const 
  {
    // Points[0 .. 2]
    glm::vec4 c = {color.r, color.g, color.b, color.a};
    glm::vec2 point1 = {points[0].x,points[0].y};
    glm::vec2 point2 = {points[1].x, points[1].y};
    glm::vec2 point3 = {points[2].x, points[2].y};
    
    b.DrawTriangle(point1, point2, point3, c);

    if(color != outLine)
    {
      glm::vec4 outColor = {outLine.r, outLine.g, outLine.b, outLine.a};
      
      b.DrawLine(point1, point2, thickness, outColor);
      b.DrawLine(point2, point3, thickness, outColor);
      b.DrawLine(point3, point1, thickness, outColor);
    }
  }

  void Triangle::Update(float dt)
  {
    (void)dt;
  }

  // Polygon part
  Polygon::Polygon(float x, float y, int sides, const Color& _color, float _thickness)
    : Object{"Polygon " + std::to_string(amountGeometries)}, position(x,y), sides(sides), color(_color), outLine(_color), thickness(_thickness)
  {
    if(sides < 0)
    {
      sides = 0;
    }
    poolPoints.reserve(sides); 

    amountGeometries ++;
    LOG_INFO(" | << Object: Polygon created, AmountGeometries " + std::to_string(amountGeometries));
  }

  Polygon::Polygon(const Vector2& _position, int sides, const Color& _color, float _thickness)
    : Object{"Polygon " + std::to_string(amountGeometries)}, position(_position), sides(sides), color(_color), outLine(_color), thickness(_thickness)
  {
    if(sides < 0)
    {
      sides = 0;
    }
    poolPoints.reserve(sides); 

    amountGeometries ++;
    LOG_INFO(" | << Object: Polygon created, AmountGeometries " + std::to_string(amountGeometries));
  }

  Polygon::Polygon(const Vector2& _position, int _sides, const Color& _color, const Color& _outline,  float _thickness)
    : Object{"Polygon " + std::to_string(amountGeometries)}, position{_position}, sides(_sides), color(_color), outLine{_outline}, thickness {_thickness}
  {
    if(sides < 0)
    {
      sides = 0;
    }
    poolPoints.reserve(sides + 1); 

    amountGeometries ++;
    LOG_INFO(" | << Object: Polygon created, AmountGeometries " + std::to_string(amountGeometries));

  }

  Polygon::Polygon(float x, float y, int _sides, const Color& _color, const Color& _outline,  float _thickness)
    : Object{"Polygon " + std::to_string(amountGeometries)}, position{x,y}, sides(_sides), color(_color), outLine{_outline}, thickness {_thickness}
  {
    if(sides < 0)
    {
      sides = 0;
    }
    poolPoints.reserve(sides + 1); 

    amountGeometries ++;
    LOG_INFO(" | << Object: Polygon created, AmountGeometries " + std::to_string(amountGeometries));

  }

  void Polygon::AddPoint(float x, float y)
  {
    this->poolPoints.push_back({x,y});
  }

  void Polygon::Draw(Batcher& b) const
  {
    if(poolPoints.size() < 3) return;

    glm::vec4 c = {color.r, color.g, color.b, color.a};

    b.DrawPolygon(poolPoints, c);

    if(color != outLine)
      DrawOutLine(b);
  }

  void Polygon::DrawOutLine(Batcher& b) const
  { 
    std::vector<glm::vec2> points = poolPoints;

    if(points.size() < sides+1)
    {
      points.push_back(points[0]);
    }

    for(int i=0; i<points.size()-1; i++)
    {
      glm::vec2 point = {points[i].x, points[i].y};
      glm::vec2 pointNext = {points[i+1].x, points[i+1].y};
      glm::vec4 outColor = {outLine.r, outLine.g, outLine.b, outLine.a};
      b.DrawLine(point,pointNext, thickness, outColor);
    }
  }
  
  void Polygon::Update(float dt)
  {
    (void)dt;
  }
}
