#pragma once

#include "Shape.h"
#include "BBox.h"
#include <vector>
#include <memory>

typedef std::vector<SharedShapePtr>::iterator ShapePtrVectorIterator;

class Bvh : public Shape
{
public:
  Bvh(){};
  Bvh(std::vector<SharedShapePtr> &objects);
  Bvh(SharedShapePtr& s1, SharedShapePtr& s2);
  Bvh(SharedShapePtr& s1, SharedShapePtr& s2, BBox &bbox_in);

  bool GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation *info);
  bool DoesHit(const Ray &ray, const Real tMax, const Real tMin);
  Vec3 GetNormalVector(const Vec3 &hitPoint);
  BBox BoundingBox(){return this->Bbox;}
  
private:
  
  SharedShapePtr BuildBranch(ShapePtrVectorIterator begin, ShapePtrVectorIterator end, ShapePtrVectorIterator last, int depth);
  SharedShapePtr Left;
  SharedShapePtr Right;
  BBox Bbox;
};
