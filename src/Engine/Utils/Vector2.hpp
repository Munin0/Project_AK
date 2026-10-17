// | -------------------------------
#pragma once
// | -------------------------------
#include "glm/ext/vector_float2.hpp"
// | -------------------------------
#include "nlohmann/detail/macro_scope.hpp"
#include "nlohmann/json.hpp"
// | -------------------------------
#include <cmath>
#include <string>
// | -------------------------------

namespace ENG
{
  /**
   * @brief 2D vector of floats.
   *
   * Vector2 stores an { x, y } pair and provides the basic arithmetic,
   * comparison and utility operations used across the engine for positions,
   * sizes, directions and velocities.
   *
   * It can be serialized to and from JSON (nlohmann::json) as
   * `{ "x": ..., "y": ... }`, and it converts implicitly to glm::vec2.
   *
   * @note Comparisons use exact floating-point equality (no epsilon).
   */
  class Vector2
  {
    public:
      /**
       * @brief Generates the JSON conversion functions for Vector2.
       *
       * Enables `nlohmann::json j = vec;` and `vec = j.get<Vector2>();` using
       * the members `x` and `y`. Both keys are required when reading.
       */
      NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector2, x, y)
      /**
       * @brief Default constructor.
       *
       * @warning The components are left uninitialized.
       */
      Vector2() {}
      /**
       * @brief Creates a vector with the same value in both components.
       *
       * @param[in] _ Value assigned to both x and y.
       */
      explicit Vector2(float _)
        : x(_), y(_) {}
      /**
       * @brief Creates a vector from two components.
       *
       * @param[in] x Value of the x component.
       * @param[in] y Value of the y component.
       */
      Vector2(float x, float y)
        : x(x), y(y) {}
      /**
       * @brief Sets both components to the same value.
       *
       * @param[in] _ Value assigned to both x and y.
       */
      void SetVector(float _)
      {
        this->x = _;
        this->y = _;
      }
      /**
       * @brief Sets both components.
       *
       * @param[in] x New value of the x component.
       * @param[in] y New value of the y component.
       */
      void SetVector(float x, float y)
      {
        this->x = x;
        this->y = y;
      }
      /**
       * @brief Normalizes this vector in place.
       *
       * After the call the vector has a length of 1 and keeps its direction.
       * A zero-length vector is left unchanged.
       */
      Vector2 Normalize()
      {
        float length = Length();
        if(length != 0.0f)
        {
          this->x /= length;
          this->y /= length;
        }
        return *this;
      }
      /**
       * @brief Returns the length (magnitude) of ~this vector.
       *
       * @warning Despite being a member function, it does not use this
       *          vector: it computes the length of @p ~this only, not the
       *          distance between this vector and @p ~this.
       *
       * @return The length of @p ~this, that is sqrt(~this.x^2 + ~thisy^2).
       */
      float Length() const
      {
        return std::sqrt(this->x*this->x + this->y*this->y);
      }
      /**
       * @brief Implicit conversion to glm::vec2.
       *
       * @return A glm::vec2 with the same x and y values.
       */
      operator glm::vec2() const
      {
        return glm::vec2{x, y};
      }
      /**
       * @brief Copy assignment.
       *
       * @param[in] vec Vector to copy from.
       * @return Reference to this vector.
       */
      Vector2& operator=(const Vector2& vec) = default;
      /// @name Arithmetic operators (Vector2 with Vector2)
      /// @{
      /**
       * @brief Component-wise multiplication.
       *
       * @param[in] v1 Right-hand operand.
       * @return { x * v1.x, y * v1.y }.
       */
      Vector2 operator*(const Vector2& v1) const
      {
        return {this->x*v1.x,this->y*v1.y};
      }
      /**
       * @brief Component-wise addition.
       *
       * @param[in] v1 Right-hand operand.
       * @return { x + v1.x, y + v1.y }.
       */
      Vector2 operator+(const Vector2& v1) const
      {
        return {this->x+v1.x,this->y+v1.y};
      }
      /**
       * @brief Component-wise subtraction.
       *
       * @param[in] v1 Right-hand operand.
       * @return { x - v1.x, y - v1.y }.
       */
      Vector2 operator-(const Vector2& v1) const
      {
        return {x-v1.x , y-v1.y};
      }
      /// @}

      /// @name Arithmetic operators (Vector2 with scalar)
      /// @{
      /**
       * @brief Divides both components by a scalar.
       *
       * @warning Instead of producing infinities or NaN, this operator
       *          returns { 0, 0 } when @p value is 0, and also when either
       *          component of this vector is 0 (even if the other one is
       *          not).
       *
       * @param[in] value Divisor.
       * @return { x / value, y / value }, or { 0, 0 } in the cases above.
       */
      Vector2 operator/(float value) const
      {
        if(value == 0)
            return {0.0f,0.0f};
        if(this->x == 0.0f && this->y != 0.0f)
          return {0.0f, this->y / value};
        if(this->x != 0.0f && this->y == 0.0f)
          return {this->x / value, 0.0f};
        if(this->x == 0 && this->y == 0)
          return {0.0f, 0.0f};

        return {this->x/value, this->y/value};
      }
      /**
       * @brief Multiplies both components by a scalar.
       *
       * @param[in] value Scalar factor.
       * @return { x * value, y * value }.
       */
      Vector2 operator*(float value) const
      {
        return {this->x*value, this->y*value};
      }
      /**
       * @brief Adds a scalar to both components.
       *
       * @param[in] value Scalar to add.
       * @return { x + value, y + value }.
       */
      Vector2 operator+(float value) const
      {
        return {this->x+value, this->y+value};
      }
      /**
       * @brief Subtracts a scalar from both components.
       *
       * @param[in] value Scalar to subtract.
       * @return { x - value, y - value }.
       */
      Vector2 operator-(float value) const
      {
        return {this->x-value, this->y-value};
      }
      /// @}

      /// @name Comparison operators
      /// @{
      /**
       * @brief Equality comparison.
       *
       * @param[in] v Vector to compare with.
       * @return true if both x and y are exactly equal.
       */
      bool operator==(const Vector2& v) const
      {
        return (this->x == v.x && this->y == v.y);
      }
      /**
       * @brief Inequality comparison.
       *
       * @param[in] v Vector to compare with.
       * @return true if either x or y differs from v.
       */
      bool operator!=(const Vector2& v) const
      {
        return !(*this == v);
      }
      /**
       * @brief Component-wise "less than".
       *
       * @note This is a partial order: two vectors can be neither
       *       less than, greater than nor equal to each other
       *       (e.g. { 1, 2 } and { 2, 1 }). Do not use it as a sorting
       *       criterion or as the key comparator of ordered containers.
       *
       * @param[in] v Vector to compare with.
       * @return true if x < v.x and y < v.y.
       */
      bool operator<(const Vector2& v) const
      {
        return (this->x < v.x && this->y < v.y);
      }
      /**
       * @brief Component-wise "less than or equal".
       *
       * @param[in] v Vector to compare with.
       * @return true if x <= v.x and y <= v.y.
       * @see operator<() for the note about partial ordering.
       */
      bool operator<=(const Vector2& v) const
      {
        return (this->x <= v.x && this->y <= v.y);
      }
      /**
       * @brief Component-wise "greater than".
       *
       * @param[in] v Vector to compare with.
       * @return true if x > v.x and y > v.y.
       * @see operator<() for the note about partial ordering.
       */
      bool operator>(const Vector2& v) const
      {
        return (this->x > v.x && this->y > v.y);
      }
      /**
       * @brief Component-wise "greater than or equal".
       *
       * @param[in] v Vector to compare with.
       * @return true if x >= v.x and y >= v.y.
       * @see operator<() for the note about partial ordering.
       */
      bool operator>=(const Vector2& v) const
      {
        return (this->x >= v.x && this->y >= v.y);
      }
      /// @}

      /// @name Compound assignment operators
      /// @{
      /**
       * @brief Adds a scalar to both components of this vector.
       *
       * @param[in] _inc Scalar to add.
       * @return Reference to this vector.
       */
      Vector2& operator+=(float _inc)
      {
        this->x += _inc;
        this->y += _inc;
        return *this;
      }
      /**
       * @brief Adds another vector to this vector, component by component.
       *
       * @param[in] v Vector to add.
       * @return Reference to this vector.
       */
      Vector2& operator+=(const Vector2& v)
      {
        this->x += v.x;
        this->y += v.y;
        return *this;
      }
      /**
       * @brief Subtracts a scalar from both components of this vector.
       *
       * @param[in] _dec Scalar to subtract.
       * @return Reference to this vector.
       */
      Vector2& operator-=(float _dec)
      {
        this->x -= _dec;
        this->y -= _dec;
        return *this;
      }
      /**
       * @brief Multiplies this vector component-wise by another vector.
       *
       * @param[in] v Vector to multiply by.
       * @return Reference to this vector.
       */
      Vector2& operator*=(const Vector2& v)
      {
        this->x *= v.x;
        this->y *= v.y;
        return *this;
      }
      /**
       * @brief Divides this vector component-wise by another vector.
       *
       * @param[in] v Vector to divide by.
       * @return Reference to this vector.
       */
      Vector2& operator/=(const Vector2& v)
      {
        if (v.x != 0.0f)
          this->x /= v.x;
        if (v.y != 0.0f)
          this->y /= v.y;
        return *this;
      }
      /**
       * @brief Subtracts another vector from this vector, component by component.
       *
       * @param[in] v Vector to subtract.
       * @return Reference to this vector.
       */
      Vector2& operator-=(const Vector2& v)
      {
        this->x -= v.x;
        this->y -= v.y;
        return *this;
      }
      /// @}
      
      std::string ToString(void) const
      {
        return std::to_string(this->x) + " " + std::to_string(this->y);
      }
    public:
      /// @brief Horizontal component.
      float x;
      /// @brief Vertical component.
      float y;
  };
}
