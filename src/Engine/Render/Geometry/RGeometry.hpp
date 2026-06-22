// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Object/Object.hpp"
// | -------------------------------
#include <glm/ext/vector_float2.hpp>
// | -------------------------------
#include <vector>
// | -------------------------------
#define RECTANGLE_SIZE_LINES 4
#define TRIANGLE_SIZE_LINES 3
// | -------------------------------

namespace ENG
{
  // Forward
  class Batcher;

  class Line : public Object 
  {
    public:
      Line(float x, float y,float xw, float yh, const Color& c);
      Line(const Vector2& start, const Vector2& end, const Color& c);
      ~Line() override {}

      void Draw(Batcher& batching) const override;
      void Update(float dt) override;
    private:
      Vector2 pStart;
      Vector2 pEnd;
      Color color;
  };

  class Rectangle : public Object 
  {
    public:
      Rectangle(float x, float y, float w, float h, const Color& c);
      Rectangle(float x, float y, float w, float h, const Color& c, const Color& outColor, float _thickness = 1.0f);
      Rectangle(float xy, float wh, const Color& c);
      Rectangle(float xy, float wh , const Color& c, const Color& outColor, float _thickness = 1.0f);
      Rectangle(const Vector2& position, const Vector2& size, const Color& c);
      Rectangle(const Vector2& position, const Vector2& size, const Color& c, const Color& outColor, float _thickness = 1.0f);
      ~Rectangle() override {}
  
      void Draw(Batcher& batching) const override;
      void Update(float dt) override;

      private:
      Vector2 position;
      Vector2 size;
      float thicknes;
      Color color;
      Color line_color;
  };

  class Circle : public Object
  {
    public:
      Circle(float x, float y, float diameter, const Color& c);
      Circle(float x, float y, float diameter, const Color& c, const Color& outLine, float _thickness = 1.0f);
      Circle(const Vector2& position, float diameter, const Color& c);
      Circle(const Vector2& position, float diameter, const Color& c, const Color& outLine, float _thickness = 1.0f);
      ~Circle() override {}

      void Draw(Batcher& batching) const override;
      void Update(float dt) override;
    private:
      float diameter = 0;
      float thickness;
      Vector2 position;
      Color color;
      Color line_color;
  };

  class Triangle : public Object
  {
    public:
      Triangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& _color);
      Triangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& _color, const Color& _outLine, float _thickness = 1.0f);
      ~Triangle() override {}

      void Draw(Batcher& batching) const override;
      void Update(float dt) override;
    private:
      float thickness;
      Vector2 points[4];
      Vector2 position;
      Color color;
      Color outLine;
  };

  class Polygon : public Object
  {
    public:
      Polygon(float x, float y, int _sides, const Color& _color, float _thickness = 1.0f);
      Polygon(const Vector2& _position, int _sides, const Color& _color, float _thickness = 1.0f);
      Polygon(float x, float y, int _sides, const Color& _color, const Color& _outline,  float _thickness = 1.0f);
      Polygon(const Vector2& _position, int _sides, const Color& _color, const Color& _outline,  float _thickness = 1.0f);
      ~Polygon() override {}
    
      void Draw(Batcher& batching) const override;
      void DrawOutLine(Batcher& b) const;
      void Update(float dt) override;
      
      void AddPoint(float x, float y);
    private:
      int sides;
      float thickness;
      Vector2 position;
      std::vector<glm::vec2> poolPoints;
      Color color;
      Color outLine;
  };
  // Class multi size, like 5 lados.
}
