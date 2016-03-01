#pragma once
#include <memory>
#include <string>

#include "Vec3.h"
#include "Ray.h"
#include "RealTypeDef.h"
#include "RandomNumberGenerator.h"


/////////////////
// Type def
/////////////////
class Material;
typedef std::shared_ptr<Material> SharedMaterialPtr;


/////////////////
// Parameter def
/////////////////
enum MaterialType
{
  SIMPLE_MATERIAL  = 0,
  SCHLICK_MATERIAL = 1,
  TEXTURE_MATERIAL = 2
};


enum ReflectionType
{
  DIFFUSE    = 0,
  SPECULAR   = 1,
  REFRACTION = 2
};


struct MaterialParam
{
  //Simple
  MaterialParam(ReflectionType rType, Vec3 color, Real reflectance, Real refractive)
    : RType(rType), Color(color), Reflectance(reflectance), Refractive(refractive)
  {
    this->MType =SIMPLE_MATERIAL;
  }

  //Schlick
  MaterialParam(){}

  MaterialType MType;

  //Simple Material
  ReflectionType RType;
  Vec3 Color;
  Real Reflectance;
  Real Refractive;

  //Schlick Material
};




////////////////////////
//
// Material(interface)
//
////////////////////////
class Material
{

public:
  static SharedMaterialPtr CreateMaterial(const MaterialParam& param);
 
  virtual ReflectionType Interaction(UniformRandomGenerator01 &rand, const Ray &inRay, Ray *outRay){}
  virtual Vec3 GetColor(){}
  virtual ReflectionType GetReflectionType(){}
  virtual Real GetRefractive(){}
  virtual Real GetReflectance(){}
  
private:

};



///////////////////////
//
// Simple Material
//
///////////////////////
class SimpleMaterial : public Material
{
public:
  SimpleMaterial(const MaterialParam &param)
    :RType(param.RType), Color(param.Color),
     Reflectance(param.Reflectance), Refractive(param.Refractive)
  {}

  ReflectionType Interaction(UniformRandomGenerator01 &rand, const Ray &inRay, Ray *outRay);
  
  Vec3 GetColor(){ return Color; }

  ReflectionType GetReflectionType(){ return this->RType; }

  Real GetRefractive(){ return this->Refractive;}

  Real GetReflectance(){ return this->Reflectance;}

private:
  ReflectionType RType;
  Vec3 Color;
  Real Reflectance;
  Real Refractive;
};




///////////////////////
//
// Schlick Material
//
///////////////////////
