// | -------------------------------
#include "Geometry.hpp"
// | -------------------------------
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Color/Color.hpp"
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
  inline static int s_amountGeometries = 0;
  /// Line Part
  GLine::GLine(float x, float y, float xw, float yh, const Color& c)
    : Object{"Line " + std::to_string(s_amountGeometries)}, m_pStart(x, y), m_pEnd(xw, yh), m_color(c)
  {
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Line created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  GLine::GLine(const Vector2& start, const Vector2& end, const Color& c, float thickness)
    : Object{"Line " + std::to_string(s_amountGeometries)}, m_pStart(start), m_pEnd(end), m_color(c), m_thickness(thickness)
  {
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Line created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  void GLine::Draw(Batcher& b) const
  {
    glm::vec2 pointA = {m_pStart.x, m_pStart.y};
    glm::vec2 pointB = {m_pEnd.x, m_pEnd.y};
    glm::vec4 c = {m_color.m_r, m_color.m_g, m_color.m_b, m_color.m_a};

    b.DrawLine(pointA, pointB, m_thickness, c);
  }
  
  void GLine::Update(float dt)
  {
    (void)dt;
  }

  /// Rectangle part
  GRectangle::GRectangle(float x, float y, float w, float h, const Color& c)
    : Object("Rectangle " + std::to_string(s_amountGeometries)), m_size(w,h), m_position(x,y), m_color(c), m_line_color(c)
  { 
    this->GetTransform().m_position = {x,y};
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }
  GRectangle::GRectangle(float x, float y, float w, float h, const Color& c, const Color& outColor, float _thickness)
    : Object{"Rectangle " + std::to_string(s_amountGeometries)}, m_position(x,y),m_size(w,h),m_color(c), m_line_color(outColor), m_thickness{_thickness}
  { 
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  GRectangle::GRectangle(float xy, float wh, const Color& c)
    : Object{"Rectangle " + std::to_string(s_amountGeometries)}, m_size(wh, wh), m_position(xy,xy), m_color(c), m_line_color(c)
  {
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  GRectangle::GRectangle(float xy, float wh, const Color& c, const Color& outColor, float _thickness)
    : Object{"Rectangle " + std::to_string(s_amountGeometries)}, m_size(wh,wh), m_position(xy,xy), m_color(c), m_line_color(outColor), m_thickness{_thickness}
  {
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  GRectangle::GRectangle(const Vector2& position, const Vector2& size, const Color& c)
    : Object{"Rectangle " + std::to_string(s_amountGeometries)}, m_size(size), m_position(position), m_color{c}, m_line_color{c} 
  {
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  GRectangle::GRectangle(const Vector2& position, const Vector2& size, const Color& c, const Color& outColor, float _thickness)
    : Object{"Rectangle " + std::to_string(s_amountGeometries)}, m_size(size), m_position(position), m_color{c}, m_line_color{outColor}, m_thickness{_thickness}
  {
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Rectangle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  void GRectangle::Draw(Batcher& b) const
  {
    // Fill
    glm::vec4 fillColor = {m_color.m_r, m_color.m_g, m_color.m_b, m_color.m_a};
    glm::vec2 pos = {m_position.x, m_position.y};
    glm::vec2 size = {this->m_size.x, this->m_size.y};

    b.DrawQuad(pos,size,fillColor);

    // Outline
    if(m_line_color != m_color)
    {
      float x = m_position.x, y = m_position.y;
      float w = size.x, h = size.y;
      
      glm::vec4 outColor = {m_line_color.m_r, m_line_color.m_g, m_line_color.m_b, m_line_color.m_a};
      b.DrawQuad({x, y}, {w, m_thickness}, outColor);                                                // Top
      b.DrawQuad({x, y + h - m_thickness}, {w, m_thickness}, outColor);                              // Bottom
      b.DrawQuad({x, y + m_thickness}, {m_thickness, h - m_thickness * 2}, outColor);                   // Left
      b.DrawQuad({x + w - m_thickness, y + m_thickness}, {m_thickness, h - m_thickness * 2}, outColor); // Right
    }
  }

  void GRectangle::Update(float dt)
  {
  }

  /// Circle part
  GCircle::GCircle(float x, float y, float _diameter, const Color& _color)
    : Object{"Circle " + std::to_string(s_amountGeometries)}, m_position(x, y), m_diameter{_diameter}, m_color{_color}
  {
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Circle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  GCircle::GCircle(float x, float y, float _diameter, const Color& _color, const Color& outLine, float _thickness)
    : Object{"Circle " + std::to_string(s_amountGeometries)}, m_position{x, y}, m_diameter{_diameter}, m_color{_color}, m_line_color{outLine}, m_thickness{_thickness}
  {
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Circle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  GCircle::GCircle(const Vector2& _position, float _diameter, const Color& _color)
    : Object{"Circle " + std::to_string(s_amountGeometries)}, m_position{_position}, m_diameter{_diameter}, m_color{_color}
  {
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Circle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }
  GCircle::GCircle(const Vector2& _position, float _diameter, const Color& _color, const Color& outLine, float _thickness)
    : Object{"Circle " + std::to_string(s_amountGeometries)}, m_position{_position}, m_diameter{_diameter}, m_color{_color}, m_line_color{outLine}, m_thickness{_thickness}
  {
    s_amountGeometries ++;
    LOG_INFO(" | << Object: Circle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  void GCircle::Draw(Batcher& b) const
  {
    float rad = m_diameter * 0.5f;

    glm::vec2 pos= {m_position.x, m_position.y};
    glm::vec4 c = {m_color.m_r, m_color.m_g, m_color.m_b, m_color.m_a};

    b.DrawCircle(pos, rad, c);

    glm::vec4 oColor = {m_line_color.m_r, m_line_color.m_g, m_line_color.m_b, m_line_color.m_a};
    if(m_line_color != m_color)
      b.DrawCircleOutline(pos, rad, oColor, m_thickness);
  }

  void GCircle::Update(float dt)
  {
    (void)dt;
  }

  /// Triangle part
  GTriangle::GTriangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& _color)
    : Object{"Triangle " + std::to_string(s_amountGeometries)}, m_color(_color), m_outline{_color}
  {
    m_points[0] = a;
    m_points[1] = b;
    m_points[2] = c;

    s_amountGeometries ++;
    LOG_INFO(" | << Object: Triangle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }
  
  GTriangle::GTriangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& color, const Color& outline, float _thickness)
    : Object{"Triangle " + std::to_string(s_amountGeometries)}, m_color(color), m_outline{outline}, m_thickness{_thickness}
  {
    m_points[0] = a;
    m_points[1] = b;
    m_points[2] = c;

    s_amountGeometries ++;
    LOG_INFO(" | << Object: Triangle created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  void GTriangle::Draw(Batcher& b) const 
  {
    // Points[0 .. 2]
    glm::vec4 c = {m_color.m_r, m_color.m_g, m_color.m_b, m_color.m_a};
    glm::vec2 point1 = {m_points[0].x, m_points[0].y};
    glm::vec2 point2 = {m_points[1].x, m_points[1].y};
    glm::vec2 point3 = {m_points[2].x, m_points[2].y};
    
    b.DrawTriangle(point1, point2, point3, c);

    if(m_color != m_outline)
    {
      glm::vec4 outColor = {m_outline.m_r, m_outline.m_g, m_outline.m_b, m_outline.m_a};
      
      b.DrawLine(point1, point2, m_thickness, outColor);
      b.DrawLine(point2, point3, m_thickness, outColor);
      b.DrawLine(point3, point1, m_thickness, outColor);
    }
  }

  void GTriangle::Update(float dt)
  {
    (void)dt;
  }

  // Polygon part
  GPolygon::GPolygon(float x, float y, int sides, const Color& _color, float _thickness)
    : Object{"Polygon " + std::to_string(s_amountGeometries)}, m_position(x,y), m_sides(sides), m_color(_color), m_outline(_color), m_thickness(_thickness)
  {
    if(sides < 0)
    {
      sides = 0;
    }
    m_poolPoints.reserve(sides); 

    s_amountGeometries ++;
    LOG_INFO(" | << Object: Polygon created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  GPolygon::GPolygon(const Vector2& _position, int sides, const Color& _color, float _thickness)
    : Object{"Polygon " + std::to_string(s_amountGeometries)}, m_position(_position), m_sides(sides), m_color(_color), m_outline(_color), m_thickness(_thickness)
  {
    if(sides < 0)
    {
      sides = 0;
    }
    m_poolPoints.reserve(sides); 

    s_amountGeometries ++;
    LOG_INFO(" | << Object: Polygon created, AmountGeometries " + std::to_string(s_amountGeometries));
  }

  GPolygon::GPolygon(const Vector2& _position, int _sides, const Color& _color, const Color& _outline,  float _thickness)
    : Object{"Polygon " + std::to_string(s_amountGeometries)}, m_position{_position}, m_sides(_sides), m_color(_color), m_outline{_outline}, m_thickness {_thickness}
  {
    if(m_sides < 0)
    {
      m_sides = 0;
    }
    m_poolPoints.reserve(m_sides + 1); 

    s_amountGeometries ++;
    LOG_INFO(" | << Object: Polygon created, AmountGeometries " + std::to_string(s_amountGeometries));

  }

  GPolygon::GPolygon(float x, float y, int _sides, const Color& _color, const Color& _outline,  float _thickness)
    : Object{"Polygon " + std::to_string(s_amountGeometries)}, m_position{x,y}, m_sides(_sides), m_color(_color), m_outline{_outline}, m_thickness {_thickness}
  {
    if(m_sides < 0)
    {
      m_sides = 0;
    }
    m_poolPoints.reserve(m_sides + 1); 

    s_amountGeometries ++;
    LOG_INFO(" | << Object: Polygon created, AmountGeometries " + std::to_string(s_amountGeometries));

  }

  void GPolygon::AddPoint(float x, float y)
  {
    this->m_poolPoints.push_back({x,y});
  }

  void GPolygon::Draw(Batcher& b) const
  {
    if(m_poolPoints.size() < 3) return;

    glm::vec4 c = {m_color.m_r, m_color.m_g, m_color.m_b, m_color.m_a};

    b.DrawPolygon(m_poolPoints, c);

    if(m_color != m_outline)
      DrawOutLine(b);
  }

  void GPolygon::DrawOutLine(Batcher& b) const
  { 
    std::vector<glm::vec2> points = m_poolPoints;

    if(points.size() < m_sides+1)
    {
      points.push_back(points[0]);
    }

    for(int i=0; i<points.size()-1; i++)
    {
      glm::vec2 point = {points[i].x, points[i].y};
      glm::vec2 pointNext = {points[i+1].x, points[i+1].y};
      glm::vec4 outColor = {m_outline.m_r, m_outline.m_g, m_outline.m_b, m_outline.m_a};
      b.DrawLine(point,pointNext, m_thickness, outColor);
    }
  }
  
  void GPolygon::Update(float dt)
  {
    (void)dt;
  }
}
