#pragma once

#include "RealTypeDef.h"
#include "Vec3.h"
#include <cmath>

struct Ray
{
  Ray(){};
  Ray(const Vec3 &origin, const Vec3 &direction) : Origin(origin)
  {
    Direction =direction.Normalize();
  }

  Vec3 Origin;
  Vec3 Direction; 
};
