#pragma once

#include "RandomNumberGenerator.h"
#include "Ray.h"
#include "HitInformation.h"
#include "Material.h"
#include "CoordinateConversion.h"

#include <cassert>
#include <cmath>
#include <iostream>




inline Ray UpdateRayDiffuse(UniformRandomGenerator01 &random, const Ray &ray, const HitInformation &info)
{
  Vec3 tmpHitPos =ray.Origin +ray.Direction *(info.RayParam *0.999);
  Vec3 normal =info.NormalVector;
  Vec3 orientNormal =normal.Dot(ray.Direction) < 0.0 ? normal : normal*(-1.0) ;
  SphericalCoordinateAngle_t displace(random.GenRand()*M_PI/2.0, random.GenRand()*M_PI*2.0);
  Vec3 direction =ChangeDirectionBySphericalCoordinateAngle(info.NormalVector, displace);
  return Ray(tmpHitPos, direction.Normalize() );
}


inline Ray UpdateRaySpecular(const Ray &ray, const HitInformation &info)
{
  Vec3 tmpHitPos =ray.Origin +ray.Direction *(info.RayParam *0.999);
  Vec3 normal =info.NormalVector;
  Vec3 reflectRayDirection =ray.Direction -normal *2.0 *normal.Dot(ray.Direction);
  return Ray(tmpHitPos, reflectRayDirection.Normalize() );
}



inline Ray UpdateRayRefractionIntruder(const Ray &ray, const HitInformation &info)
{
  Vec3 normal =info.NormalVector;
  Vec3 orientNormal =normal.Dot(ray.Direction) < 0.0 ? normal : normal*(-1.0) ;
  bool isIncidence = normal.Dot(orientNormal) > 0.0;
  Real eta0 =1.0; //vacuum
  Real eta =info.MaterialPtr->GetRefractive();
  Real etaRatio =isIncidence ? eta0/eta : eta/eta0;
  Real cos_t =orientNormal.Dot(ray.Direction);
  Real cos_2t = 1.0 -etaRatio *etaRatio *(1.0 -cos_t *cos_t);

  Vec3 refractRayDirection =(ray.Direction *etaRatio -normal *(isIncidence ? 1.0 : -1.0)
			       *(cos_t *etaRatio +sqrt(cos_2t))).Normalize(); //update ray

  Vec3 tmpHitPos =ray.Origin +ray.Direction *(info.RayParam *1.001);

  return Ray(tmpHitPos, refractRayDirection);
}



inline Ray UpdateRayRefraction(UniformRandomGenerator01 &random, const Ray &ray, const HitInformation &info)
{
  Vec3 normal =info.NormalVector;
  Vec3 orientNormal =normal.Dot(ray.Direction) < 0.0 ? normal : normal*(-1.0) ;
  bool isIncidence = normal.Dot(orientNormal) > 0.0;
  Real eta0 =1.0; //vacuum's refractive
  Real eta =info.MaterialPtr->GetRefractive();
  Real etaRatio =isIncidence ? eta0/eta : eta/eta0;
  Real cos_t =orientNormal.Dot(ray.Direction);
  Real cos_2t = 1.0 -etaRatio *etaRatio *(1.0 -cos_t *cos_t);
	  
  Vec3 reflectRayDirection =ray.Direction -normal *2.0 *normal.Dot(ray.Direction); //update ray
  
  if(cos_2t < 0.0){ //reflection
    Vec3 tmpHitPos =ray.Origin +ray.Direction *(info.RayParam *0.999);
    return Ray(tmpHitPos, reflectRayDirection.Normalize() );

  }else{
    Vec3 refractRayDirection =(ray.Direction -orientNormal *cos_t)*etaRatio -orientNormal *sqrt(cos_2t); 

    //Fresnel coef
    Real a =eta -eta0;
    Real b =eta +eta0;
    Real F_real =(a*a)/(b*b);
    Real c =1.0 -(isIncidence ? -cos_t : refractRayDirection.Dot(orientNormal *(-1.0)));
    Real SchlickFr =F_real +(1.0 -F_real) *pow(c, 5.0);
    Real probability = 0.1 +0.8 *SchlickFr;
      
    if(random.GenRand() > probability){ //Refraction
      Vec3 tmpHitPos =ray.Origin +ray.Direction *(info.RayParam *1.001);
      return Ray(tmpHitPos, refractRayDirection);

    }else{ // Reflection
      Vec3 tmpHitPos =ray.Origin +ray.Direction *(info.RayParam *0.999);
      return Ray(tmpHitPos, reflectRayDirection.Normalize() );

    }
  }
}



inline bool DoesReflect(UniformRandomGenerator01 &random, const int ReflectionNum, const HitInformation &info)
{
  Real probability =info.MaterialPtr->GetReflectance();
  return probability > random.GenRand();
}



inline Ray UpdateRay(UniformRandomGenerator01 &random, const Ray &ray, const HitInformation &info)
{
  switch(info.MaterialPtr->GetReflectionType()){
  case DIFFUSE:{
    return UpdateRayDiffuse(random, ray, info);
    break;
  }
  case SPECULAR:{
    return UpdateRaySpecular(ray, info);
    break;
  }
  case REFRACTION:{
    return UpdateRayRefraction(random, ray, info);
    break;
  }
  default:{
    assert(!"invalid refrection type");
    break;
  }
  }
}

