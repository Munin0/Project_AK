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

  class GLine : public Object 
  {
    public:
      GLine(float x, float y,float xw, float yh, const Color& c);
      GLine(const Vector2& start, const Vector2& end, const Color& c);
      ~GLine() override {}

      void Draw(Batcher& batching) const override;
      void Update(float dt) override;
    private:
      Vector2 pStart;
      Vector2 pEnd;
      Color color;
  };

  class GRectangle : public Object 
  {
    public:
      GRectangle(float x, float y, float w, float h, const Color& c);
      GRectangle(float x, float y, float w, float h, const Color& c, const Color& outColor, float _thickness = 1.0f);
      GRectangle(float xy, float wh, const Color& c);
      GRectangle(float xy, float wh , const Color& c, const Color& outColor, float _thickness = 1.0f);
      GRectangle(const Vector2& position, const Vector2& size, const Color& c);
      GRectangle(const Vector2& position, const Vector2& size, const Color& c, const Color& outColor, float _thickness = 1.0f);
      ~GRectangle() override {}
  
      void Draw(Batcher& batching) const override;
      void Update(float dt) override;

      private:
      Vector2 position;
      Vector2 size;
      float thicknes;
      Color color;
      Color line_color;
  };

  class GCircle : public Object
  {
    public:
      GCircle(float x, float y, float diameter, const Color& c);
      GCircle(float x, float y, float diameter, const Color& c, const Color& outLine, float _thickness = 1.0f);
      GCircle(const Vector2& position, float diameter, const Color& c);
      GCircle(const Vector2& position, float diameter, const Color& c, const Color& outLine, float _thickness = 1.0f);
      ~GCircle() override {}

      void Draw(Batcher& batching) const override;
      void Update(float dt) override;
    private:
      float diameter = 0;
      float thickness;
      Vector2 position;
      Color color;
      Color line_color;
  };

  class GTriangle : public Object
  {
    public:
      GTriangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& _color);
      GTriangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& _color, const Color& _outLine, float _thickness = 1.0f);
      ~GTriangle() override {}

      void Draw(Batcher& batching) const override;
      void Update(float dt) override;
    private:
      float thickness;
      Vector2 points[4];
      Vector2 position;
      Color color;
      Color outLine;
  };

  class GPolygon : public Object
  {
    public:
      GPolygon(float x, float y, int _sides, const Color& _color, float _thickness = 1.0f);
      GPolygon(const Vector2& _position, int _sides, const Color& _color, float _thickness = 1.0f);
      GPolygon(float x, float y, int _sides, const Color& _color, const Color& _outline,  float _thickness = 1.0f);
      GPolygon(const Vector2& _position, int _sides, const Color& _color, const Color& _outline,  float _thickness = 1.0f);
      ~GPolygon() override {}
    
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
