#pragma once

#include <memory>
#include <algorithm>
#include "RealTypeDef.h"
#include "Material.h"
#include "Vec3.h"
#include "Ray.h"
#include "HitInformation.h"
#include "BBox.h"

//////////////////////
//
// Type def
//
//////////////////////
class Shape;
typedef std::shared_ptr<Shape> SharedShapePtr;



/////////////////////
//
// Parameter def
//
/////////////////////
enum ShapeType
{
  SPHERE	= 0,
  TRIANGLE_FACE	= 1,
  BVH		= 2,
};


struct ShapeParam
{
  // Sphere
  ShapeParam(const Vec3& center, Real radius)
    : Type(SPHERE), Center(center), Radius(radius) {};

  ShapeType Type;

  //Sphere
  Vec3 Center;
  Real Radius;
};




///////////////////
//
// Shape(interface)
//
///////////////////
class Shape
{
public:
  static SharedShapePtr CreateShape(SharedMaterialPtr &material, const ShapeParam &param);
  static SharedShapePtr CreateTriangleFace(SharedMaterialPtr &material,
					   Vec3 &V1, Vec3 &V2, Vec3 &V3,
					   Vec3 &Vn1, Vec3 &Vn2, Vec3 &Vn3);

  virtual bool GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info){};
  virtual bool DoesHit(const Ray &ray, const Real tMax, const Real tMin){};
  virtual Vec3 GetNormalVector(const Vec3 &hitPoint){}
  virtual BBox BoundingBox(){};

  void SetMaterialPtr(SharedMaterialPtr &material){ this->Material =material;}
  SharedMaterialPtr GetMaterialPtr(){return this->Material;}
  
private:
  SharedMaterialPtr Material;
};



///////////////////
//
// Sphere
//
///////////////////
class Sphere : public Shape
{
public:
  Sphere(const Vec3& center, const Real radius) : Center(center), Radius(radius)
  {}

  bool GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info);
  bool DoesHit(const Ray &ray, const Real tMax, const Real tMin);
  Vec3 GetNormalVector(const Vec3 &hitPoint);
  BBox BoundingBox();
  
private:
  Vec3 Center;
  Real Radius;
};




///////////////////
//
// TriangleFace
//
///////////////////
class TriangleFace : public Shape
{

public:
  TriangleFace(Vec3 &v1, Vec3 &v2, Vec3 &v3,
	       Vec3 &vn1, Vec3 &vn2, Vec3 &vn3)
    : V1(v1), V2(v2), V3(v3), Vn1(vn1), Vn2(vn2), Vn3(vn3)
  {}

  bool GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info);
  bool DoesHit(const Ray &ray, const Real tMax, const Real tMin);
  Vec3 GetNormalVector(const Vec3 &hitPoint);
  BBox BoundingBox();
  
private:
  Vec3 &V1;
  Vec3 &V2;
  Vec3 &V3;
  Vec3 &Vn1;
  Vec3 &Vn2;
  Vec3 &Vn3;
};
