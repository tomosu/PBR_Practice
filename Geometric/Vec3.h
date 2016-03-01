#pragma once

#include "RealTypeDef.h"
#include <cmath>

// temporary
struct Vec3
{
  Real x, y, z;

  Vec3(const Real x_ = 0, const Real y_ = 0, const Real z_ = 0) : x(x_), y(y_), z(z_) {}
  Vec3 operator+(const Vec3 &r) const {return Vec3(x + r.x, y + r.y, z + r.z);}
  Vec3 operator-(const Vec3 &r) const {return Vec3(x - r.x, y - r.y, z - r.z);}
  Vec3 operator*(const Real r) const {return Vec3(x * r, y * r, z * r);}
  Vec3 operator/(const Real r) const {return Vec3(x / r, y / r, z / r);}
  const Real Norm2() const { return x*x + y*y + z*z; }
  const Real Length() const { return sqrt(Norm2()); }
  double Dot(const Vec3 &r) const { return x*r.x + y*r.y + z*r.z; }
  Vec3 Cross(const Vec3 &r) const { return Vec3( y*r.z - z*r.y, z*r.x - x*r.z, x*r.y - y*r.x); }
  Vec3 Multiply(const Vec3 &r) const { return Vec3(x*r.x, y*r.y, z*r.z); }
  Vec3 Normalize() const { return (*this)/Length();}
};

