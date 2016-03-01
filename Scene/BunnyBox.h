#pragma once

#include <vector>
#include <string>

#include "Ray.h"
#include "Vec3.h"
#include "Shape.h"
#include "Scene.h"
#include "Bvh.h"
#include "TriangleMesh.h"


class BunnyBox : public Scene
{

public:

  BunnyBox(){ this->BuildScene(); }
  bool GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info);

private:

  void BuildScene();
  Ray CalcNextRay(const Ray &ray, const Real rayParam, const SharedShapePtr &shape);

  SharedShapePtr ShapesBVH;
  std::vector<SharedTriangleMeshPtr> TriangleMeshes;
};