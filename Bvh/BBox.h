#pragma once

#include <memory>
#include <algorithm>
#include "Ray.h"



class BBox
{
public:
  BBox(){}
  BBox(const Vec3 &p1, const Vec3 &p2){
    pMin = Vec3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
    pMax = Vec3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
  }
  BBox(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3){
    pMin = Vec3(std::min(p3.x, std::min(p1.x, p2.x) ),
		std::min(p3.y, std::min(p1.y, p2.y) ),
		std::min(p3.z, std::min(p1.z, p2.z) ));
		
    pMax = Vec3(std::max(p3.x, std::max(p1.x, p2.x) ),
		std::max(p3.y, std::max(p1.y, p2.y) ),
		std::max(p3.z, std::max(p1.z, p2.z) ));
  }
  Vec3 Min() const {return pMin;}
  Vec3 Max() const {return pMax;}
  bool RayIntersect(const Ray &ray, const Real tMax, const Real tMin);
  bool IsInside(const Ray &ray);

private:
  Vec3 pMin;
  Vec3 pMax;
};


inline BBox Surround(const BBox &b1, const BBox &b2)
{
  Vec3 pMin, pMax;
  pMin.x = std::min(b1.Min().x, b2.Min().x);
  pMin.y = std::min(b1.Min().y, b2.Min().y);
  pMin.z = std::min(b1.Min().z, b2.Min().z);

  pMax.x = std::max(b1.Max().x, b2.Max().x);
  pMax.y = std::max(b1.Max().y, b2.Max().y);
  pMax.z = std::max(b1.Max().z, b2.Max().z);
  return BBox(pMin, pMax);
};
