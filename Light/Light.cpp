#include <cmath>
#include <iostream>
#include "Light.h"



///////////////////
//
// Interface
//
///////////////////
SharedLightPtr Light::CreateLight(const LightParam &param)
{
  switch(param.Type){
  case POINT_SOURCE:{
    //SharedLightPtr ret(new PointSource(param));
    //return ret;
    break;
  }
    
  case BALL_SOURCE:{
    SharedLightPtr ret(new BallSource(param));
    return ret;
    break;
  }
    
  default:{
    assert(!"invalid light type");
    break;
  }
  }
}



///////////////////
//
// Point Source
//
///////////////////




///////////////////
//
// Ball Source
//
///////////////////
BallSource::BallSource(const LightParam &param)
  :Center(param.Center), Radius(param.Radius)
{
  //create shape
  MaterialParam materialParam( DIFFUSE, param.Color, 0.0, 0.0 );
  SharedMaterialPtr lightMaterial =Material::CreateMaterial(materialParam);
  ShapeParam shapeParam( param.Center, param.Radius ); //ball param
  Shape =Shape::CreateShape(lightMaterial, shapeParam);
}

Vec3 BallSource::GetColor(){
  return this->Shape->GetMaterialPtr()->GetColor();
}

Vec3 BallSource::GetCenter(){
  return this->Center;
}

Real BallSource::GetRadius(){
  return this->Radius;
}

Vec3 BallSource::GetNormalVector(const Vec3 &hitpoint){
  return this->Shape->GetNormalVector(hitpoint);
}

bool BallSource::GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info)
{
  return this->Shape->GetHitInformation(ray, tMax, tMin, info);
}


Ray BallSource::GenerateRandomRay(UniformRandomGenerator01 &random){
  //gen random point on surface
  Vec3 pos =RandomOnUnitSphere(random)*this->Radius +this->Center;
  Vec3 dir =RandomOnUnitSphere(random);
  return Ray(pos, dir);
}

