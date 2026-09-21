// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/Batching/RBatch.hpp"
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
  /**
   * @brief Line object, drawn between two points.
   */
  class GLine : public Object
  {
    public:
      /**
       * @brief Creates a line from the coordinates of its two end points.
       *
       * @param[in] x  X coordinate of the start point.
       * @param[in] y  Y coordinate of the start point.
       * @param[in] xw X coordinate of the end point.
       * @param[in] yh Y coordinate of the end point.
       * @param[in] c  Color of the line.
       */
      GLine(float x, float y,float xw, float yh, const Color& c);
      /**
       * @brief Creates a line from its two end points.
       *
       * @param[in] start Start point.
       * @param[in] end   End point.
       * @param[in] c     Color of the line.
       */
      GLine(const Vector2& start, const Vector2& end, const Color& c);
      /// @brief Default destructor.
      ~GLine() override {}
      /**
       * @brief Submits the line to the Batcher for rendering.
       *
       * @param[in,out] batching Batcher that collects the draw data of this frame.
       */
      void Draw(Batcher& batching) const override;
      /**
       * @brief Updates the line once per frame.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      void Update(float dt) override;
    private:
      /// @brief Start point of the line.
      Vector2 pStart;
      /// @brief End point of the line.
      Vector2 pEnd;
      /// @brief Color of the line.
      Color color;
  };

  /**
   * @brief Rectangle object, with a fill color and an optional outline.
   */
  class GRectangle : public Object
  {
    public:
      /**
       * @brief Creates a filled rectangle.
       *
       * @param[in] x Horizontal position.
       * @param[in] y Vertical position.
       * @param[in] w Width.
       * @param[in] h Height.
       * @param[in] c Fill color.
       */
      GRectangle(float x, float y, float w, float h, const Color& c);
      /**
       * @brief Creates a filled rectangle with an outline.
       *
       * @param[in] x        Horizontal position.
       * @param[in] y        Vertical position.
       * @param[in] w        Width.
       * @param[in] h        Height.
       * @param[in] c        Fill color.
       * @param[in] outColor Outline color.
       * @param[in] _thickness Outline thickness. Defaults to 1.0.
       */
      GRectangle(float x, float y, float w, float h, const Color& c, const Color& outColor, float _thickness = 1.0f);

      /**
       * @brief Creates a filled rectangle using one value for both
       *        coordinates and one value for both dimensions.
       *
       * @param[in] xy Value used for both the x and y position.
       * @param[in] wh Value used for both the width and the height.
       * @param[in] c  Fill color.
       */
      GRectangle(float xy, float wh, const Color& c);

      /**
       * @brief Creates a filled rectangle with an outline, using one value
       *        for both coordinates and one value for both dimensions.
       *
       * @param[in] xy       Value used for both the x and y position.
       * @param[in] wh       Value used for both the width and the height.
       * @param[in] c        Fill color.
       * @param[in] outColor Outline color.
       * @param[in] _thickness Outline thickness. Defaults to 1.0.
       */
      GRectangle(float xy, float wh , const Color& c, const Color& outColor, float _thickness = 1.0f);

      /**
       * @brief Creates a filled rectangle from a position and a size.
       *
       * @param[in] position Position of the rectangle.
       * @param[in] size     Size of the rectangle: { width, height }.
       * @param[in] c        Fill color.
       */
      GRectangle(const Vector2& position, const Vector2& size, const Color& c);

      /**
       * @brief Creates a filled rectangle with an outline, from a position
       *        and a size.
       *
       * @param[in] position Position of the rectangle.
       * @param[in] size     Size of the rectangle: { width, height }.
       * @param[in] c        Fill color.
       * @param[in] outColor Outline color.
       * @param[in] _thickness Outline thickness. Defaults to 1.0.
       */
      GRectangle(const Vector2& position, const Vector2& size, const Color& c, const Color& outColor, float _thickness = 1.0f);

      /// @brief Default destructor.
      ~GRectangle() override {}

      /**
       * @brief Submits the rectangle to the Batcher for rendering.
       *
       * @param[in,out] batching Batcher that collects the draw data of this frame.
       */
      void Draw(Batcher& batching) const override;

      /**
       * @brief Updates the rectangle once per frame.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      void Update(float dt) override;

    private:
      /// @brief Position of the rectangle.
      Vector2 position;

      /// @brief Size of the rectangle: { width, height }.
      Vector2 size;

      /// @brief Thickness of the outline.
      float thicknes;

      /// @brief Fill color.
      Color color;

      /// @brief Outline color.
      Color line_color;
  };

  /**
   * @brief Circle object, with a fill color and an optional outline.
   *
   * The size of the circle is given by its diameter, not its radius.
   */
  class GCircle : public Object
  {
    public:
      /**
       * @brief Creates a filled circle.
       *
       * @param[in] x        Horizontal position.
       * @param[in] y        Vertical position.
       * @param[in] diameter Diameter of the circle.
       * @param[in] c        Fill color.
       */
      GCircle(float x, float y, float diameter, const Color& c);

      /**
       * @brief Creates a filled circle with an outline.
       *
       * @param[in] x        Horizontal position.
       * @param[in] y        Vertical position.
       * @param[in] diameter Diameter of the circle.
       * @param[in] c        Fill color.
       * @param[in] outLine  Outline color.
       * @param[in] _thickness Outline thickness. Defaults to 1.0.
       */
      GCircle(float x, float y, float diameter, const Color& c, const Color& outLine, float _thickness = 1.0f);

      /**
       * @brief Creates a filled circle from a position.
       *
       * @param[in] position Position of the circle.
       * @param[in] diameter Diameter of the circle.
       * @param[in] c        Fill color.
       */
      GCircle(const Vector2& position, float diameter, const Color& c);

      /**
       * @brief Creates a filled circle with an outline, from a position.
       *
       * @param[in] position Position of the circle.
       * @param[in] diameter Diameter of the circle.
       * @param[in] c        Fill color.
       * @param[in] outLine  Outline color.
       * @param[in] _thickness Outline thickness. Defaults to 1.0.
       */
      GCircle(const Vector2& position, float diameter, const Color& c, const Color& outLine, float _thickness = 1.0f);

      /// @brief Default destructor.
      ~GCircle() override {}

      /**
       * @brief Submits the circle to the Batcher for rendering.
       *
       * @param[in,out] batching Batcher that collects the draw data of this frame.
       */
      void Draw(Batcher& batching) const override;

      /**
       * @brief Updates the circle once per frame.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      void Update(float dt) override;

    private:
      /// @brief Diameter of the circle.
      float diameter = 0;

      /// @brief Thickness of the outline.
      float thickness;

      /// @brief Position of the circle.
      Vector2 position;

      /// @brief Fill color.
      Color color;

      /// @brief Outline color.
      Color line_color;
  };

  /**
   * @brief Triangle object, with a fill color and an optional outline.
   */
  class GTriangle : public Object
  {
    public:
      /**
       * @brief Creates a filled triangle from its three vertices.
       *
       * @param[in] a      First vertex.
       * @param[in] b      Second vertex.
       * @param[in] c      Third vertex.
       * @param[in] _color Fill color.
       */
      GTriangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& _color);

      /**
       * @brief Creates a filled triangle with an outline.
       *
       * @param[in] a        First vertex.
       * @param[in] b        Second vertex.
       * @param[in] c        Third vertex.
       * @param[in] _color   Fill color.
       * @param[in] _outLine Outline color.
       * @param[in] _thickness Outline thickness. Defaults to 1.0.
       */
      GTriangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& _color, const Color& _outLine, float _thickness = 1.0f);

      /// @brief Default destructor.
      ~GTriangle() override {}

      /**
       * @brief Submits the triangle to the Batcher for rendering.
       *
       * @param[in,out] batching Batcher that collects the draw data of this frame.
       */
      void Draw(Batcher& batching) const override;

      /**
       * @brief Updates the triangle once per frame.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      void Update(float dt) override;

    private:
      /// @brief Thickness of the outline.
      float thickness;

      /// @brief Points that define the triangle.
      Vector2 points[4];

      /// @brief Position of the triangle.
      Vector2 position;

      /// @brief Fill color.
      Color color;

      /// @brief Outline color.
      Color outLine;
  };

  /**
   * @brief Polygon object with any number of sides (for example, 5).
   *
   * The vertices of the polygon are stored in a list and can be added with
   * AddPoint().
   */
  class GPolygon : public Object
  {
    public:
      /**
       * @brief Creates a polygon at a position.
       *
       * @param[in] x          Horizontal position.
       * @param[in] y          Vertical position.
       * @param[in] _sides     Number of sides of the polygon.
       * @param[in] _color     Color of the polygon.
       * @param[in] _thickness Line thickness. Defaults to 1.0.
       */
      GPolygon(float x, float y, int _sides, const Color& _color, float _thickness = 1.0f);

      /**
       * @brief Creates a polygon at a position.
       *
       * @param[in] _position  Position of the polygon.
       * @param[in] _sides     Number of sides of the polygon.
       * @param[in] _color     Color of the polygon.
       * @param[in] _thickness Line thickness. Defaults to 1.0.
       */
      GPolygon(const Vector2& _position, int _sides, const Color& _color, float _thickness = 1.0f);

      /**
       * @brief Creates a polygon with an outline at a position.
       *
       * @param[in] x          Horizontal position.
       * @param[in] y          Vertical position.
       * @param[in] _sides     Number of sides of the polygon.
       * @param[in] _color     Fill color.
       * @param[in] _outline   Outline color.
       * @param[in] _thickness Outline thickness. Defaults to 1.0.
       */
      GPolygon(float x, float y, int _sides, const Color& _color, const Color& _outline,  float _thickness = 1.0f);

      /**
       * @brief Creates a polygon with an outline at a position.
       *
       * @param[in] _position  Position of the polygon.
       * @param[in] _sides     Number of sides of the polygon.
       * @param[in] _color     Fill color.
       * @param[in] _outline   Outline color.
       * @param[in] _thickness Outline thickness. Defaults to 1.0.
       */
      GPolygon(const Vector2& _position, int _sides, const Color& _color, const Color& _outline,  float _thickness = 1.0f);

      /// @brief Default destructor.
      ~GPolygon() override {}

      /**
       * @brief Submits the polygon to the Batcher for rendering.
       *
       * @param[in,out] batching Batcher that collects the draw data of this frame.
       */
      void Draw(Batcher& batching) const override;

      /**
       * @brief Submits only the outline of the polygon to the Batcher.
       *
       * @param[in,out] b Batcher that collects the draw data of this frame.
       */
      void DrawOutLine(Batcher& b) const;

      /**
       * @brief Updates the polygon once per frame.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      void Update(float dt) override;

      /**
       * @brief Adds a vertex to the polygon.
       *
       * @param[in] x X coordinate of the vertex.
       * @param[in] y Y coordinate of the vertex.
       */
      void AddPoint(float x, float y);

    private:
      /// @brief Number of sides of the polygon.
      int sides;

      /// @brief Thickness of the outline.
      float thickness;

      /// @brief Position of the polygon.
      Vector2 position;

      /// @brief Vertices of the polygon.
      std::vector<glm::vec2> poolPoints;

      /// @brief Fill color.
      Color color;

      /// @brief Outline color.
      Color outLine;
  };
}
