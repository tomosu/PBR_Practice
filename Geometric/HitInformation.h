#pragma once
#include <cmath>
#include "RealTypeDef.h"
#include "Material.h"
#include "Vec3.h"


struct HitInformation
{
  HitInformation(): DoesHit(false) {};
  bool DoesHit;
  Real RayParam;
  Vec3 NormalVector;
  Ray NextRay;
  SharedMaterialPtr MaterialPtr;
};
