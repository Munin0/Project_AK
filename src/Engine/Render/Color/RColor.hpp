/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "glm/ext/vector_float4.hpp"
// | -------------------------------
#include <cstdint>
/// | ------------------------------------ |

namespace ENG
{
  /**
   * @brief RGBA color with floating-point components.
   *
   * Each component (red, green, blue and alpha) is a float in the range
   * [0.0, 1.0], where 0.0 is none of that component and 1.0 is all of it.
   * The values are not clamped: it is up to the caller to keep them inside
   * the range.
   *
   * Colors can also be created from the usual 0 to 255 values with
   * From255(), and a set of common colors is available as static members
   * (Color::Red, Color::Blue, ...).
   *
   * @note Comparisons use exact floating-point equality (no epsilon).
   */
  class Color
  {
    public:
      /**
       * @brief Default constructor.
       *
       * Creates an opaque white color: { 1, 1, 1, 1 }.
       */
      constexpr Color()
        : r(1), g(1), b(1), a(1) {};

      /**
       * @brief Creates a color from its components.
       *
       * @param[in] r Red component, from 0.0 to 1.0.
       * @param[in] g Green component, from 0.0 to 1.0.
       * @param[in] b Blue component, from 0.0 to 1.0.
       * @param[in] a Alpha (opacity) component, from 0.0 (transparent) to
       *              1.0 (opaque). Defaults to 1.0.
       */
      constexpr Color(float r, float g, float b, float a = 1)
        : r(r), g(g), b(b), a(a) {};

      /**
       * @brief Sets this color from 0 to 255 component values.
       *
       * Each value is converted to the [0.0, 1.0] range used by the class.
       *
       * @param[in] _r Red component, from 0 to 255.
       * @param[in] _g Green component, from 0 to 255.
       * @param[in] _b Blue component, from 0 to 255.
       * @param[in] _a Alpha (opacity) component, from 0 to 255. Defaults to 255.
       */
      void From255(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255);

      /**
       * @brief Equality comparison.
       *
       * @param[in] c Color to compare with.
       * @return true if the four components are exactly equal.
       */
      bool operator==(const Color& c) const
      {
        return (this->r == c.r &&
            this->g == c.g &&
            this->b == c.b &&
            this->a == c.a);
      }

      /**
       * @brief Implicit conversion to glm::vec4.
       *
       * @return A glm::vec4 with the components { r, g, b, a }.
       */
      operator glm::vec4() const
      {
        return glm::vec4{this->r, this->g, this->b, this->a};
      }

      /**
       * @brief Copy assignment.
       *
       * @param[in] _color Color to copy from.
       */
      void operator=(const Color& _color)
      {
        this->r = _color.r;
        this->g = _color.g;
        this->b = _color.b;
        this->a = _color.a;
      }

    public:
      /// @brief Red component, from 0.0 to 1.0.
      float r;

      /// @brief Green component, from 0.0 to 1.0.
      float g;

      /// @brief Blue component, from 0.0 to 1.0.
      float b;

      /// @brief Alpha (opacity) component, from 0.0 (transparent) to 1.0 (opaque).
      float a;

    public:
      /// @name Predefined colors
      /// @{
      static const Color Red;     ///< Red.
      static const Color Green;   ///< Green.
      static const Color Yellow;  ///< Yellow.
      static const Color Blue;    ///< Blue.
      static const Color White;   ///< White.
      static const Color Black;   ///< Black.
      static const Color Gray;    ///< Gray.
      static const Color Blank;   ///< Blank color.
      /// @}
  };
}
