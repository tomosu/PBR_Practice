#pragma once
#include <memory>
#include <string>
#include <cassert>

#include "Shape.h"
#include "RandomNumberGenerator.h"
#include "RealTypeDef.h"

class Light;
typedef std::shared_ptr<Light> SharedLightPtr;


enum LightType
{
  POINT_SOURCE =0,
  BALL_SOURCE=1
  //SPOT_LIGHT_SOURCE
};


struct LightParam
{
  LightParam(const LightType type, const Vec3 &color, const Vec3& center,
	     const Real radius, const Real coneAngleRadian, const Vec3 lightDirection )
    :Type(type), Color(color), Center(center), Radius(radius),
     ConeAngleRadian(coneAngleRadian), LightDirection(lightDirection)
  {}

  LightType Type;
  Vec3 Color;

  //Point
  Vec3 Center;
  
  //Ball
  Real Radius;
  Real ConeAngleRadian;

  //Spot Light
  Vec3 LightDirection;
};


///////////////////
//
// Interface
//
///////////////////
class Light
{
public:

  static SharedLightPtr CreateLight(const LightParam &param);


  virtual Vec3 GetColor(){}
  virtual Vec3 GetCenter(){}
  virtual Real GetRadius(){}
  virtual Vec3 GetNormalVector(const Vec3 &hitpoint){}
  virtual bool GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info){}
  virtual Ray GenerateRandomRay(UniformRandomGenerator01 &rand){}
};



///////////////////
//
// Point Source
//
///////////////////
class PointSource : public Light
{

private:

  Vec3 Center;
  Vec3 Color;
};



///////////////////
//
// Ball Source
//
///////////////////
class BallSource : public Light
{

public:

  BallSource(const LightParam &param);

  Vec3 GetColor();
  Vec3 GetCenter();
  Real GetRadius();
  Vec3 GetNormalVector(const Vec3 &hitpoint);
  bool GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info);
  Ray GenerateRandomRay(UniformRandomGenerator01 &random);

private:

  Vec3 Center;
  Real Radius;
  SharedShapePtr Shape;
};
