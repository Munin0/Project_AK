// | -------------------------------
#pragma once
// | -------------------------------
#include <cmath>
// | -------------------------------

namespace ENG
{
  class Vector2
  {
    public:
      Vector2() {}
      Vector2(float _)
        : x(_), y(_) {}
      Vector2(float x, float y)
        : x(x), y(y) {}

      void SetVector(float _)
      {
        this->x = _;
        this->y = _;
      }
      void SetVector(float x, float y)
      {
        this->x = x;
        this->y = y;
      }
      void Normalize()
      {
        float length = std::sqrt(this->x * this->x + this->y * this->y);
        if(length != 0.0f)
        {
          this->x /= length;
          this->y /= length;
        }
      }

      float Length(const Vector2& v)
      {
        return std::sqrt(v.x*v.x + v.y*v.y);
      }
      
      // Asign
      Vector2& operator=(const Vector2& vec)
      {
        this->x = vec.x;
        this->y = vec.y;
        return *this;
      }
      /// To another Vector2
      Vector2 operator*(const Vector2& v1) const
      {
        return {this->x*v1.x,this->y*v1.y};
      }

      Vector2 operator+(const Vector2& v1) const
      {
        return {this->x+v1.x,this->y+v1.y};
      }

      Vector2 operator-(const Vector2& v1) const
      {
        return {x-v1.x , y-v1.y};
      }

      /// Decimal
      Vector2 operator/(float value) const
      {
        return {this->x/value, this->y/value};
      }
      
      Vector2 operator*(float value) const
      {
        return {this->x*value, this->y*value};
      }

      Vector2 operator+(float value) const
      {
        return {this->x+value, this->y+value};
      }

      Vector2 operator-(float value) const
      {
        return {this->x-value, this->y-value};
      }

      // Binaries
      bool operator==(const Vector2& v) const
      {
        return (this->x == v.x && this->y == v.y);
      }
      bool operator!=(const Vector2& v) const
      {
        return (this->x != v.x && this->y != v.y);
      }
      bool operator<(const Vector2& v) const
      {
        return (this->x < v.x && this->y < v.y);
      }
      bool operator<=(const Vector2& v) const
      {
        return (this->x <= v.x && this->y <= v.y);
      }
      bool operator>(const Vector2& v) const
      {
        return (this->x > v.x && this->y > v.y);
      }
      bool operator>=(const Vector2& v) const
      {
        return (this->x >= v.x && this->y >= v.y);
      }

      // Inc or Dec
      Vector2& operator+=(float _inc)
      {
        this->x += _inc;
        this->y += _inc;
        return *this;
      }
      Vector2& operator+=(const Vector2& v)
      {
        this->x += v.x;
        this->y += v.y;
      return *this;
      }
      Vector2& operator-=(float _dec)
      {
        this->x -= _dec;
        this->y -= _dec;
        return *this;
      }
      Vector2& operator-=(const Vector2& v)
      {
        this->x -= v.x;
        this->y -= v.y;
        return *this;
      }

    public:
      float x;
      float y;
  };

}
