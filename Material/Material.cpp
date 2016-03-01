#include "Material.h"
#include <cassert>


///////////////////
//
// Material(interface)
//
///////////////////
SharedMaterialPtr Material::CreateMaterial(const MaterialParam& param)
{
  switch(param.MType){
  
  case SIMPLE_MATERIAL:{
    SharedMaterialPtr ret(new SimpleMaterial(param) );
    return ret;
    break;
  }
    
  case SCHLICK_MATERIAL:{
    assert(!"not implemented\n");
    //SharedMaterialPtr ret(new SchlickMaterial(param) );
    //return ret;
    break;
  }
    
  default:{
    assert(!"invalid material type\n");
  }
  }

}


///////////////////
//
// Simple Material
//
///////////////////
ReflectionType SimpleMaterial::Interaction(UniformRandomGenerator01 &rand, const Ray &inRay, Ray *outRay)
{
  switch(this->RType){
  case DIFFUSE: break;
  case SPECULAR: break;
  case REFRACTION: break;
  }

  assert(!"not implemented\n");
  *outRay =inRay;
  return this->RType;
}




///////////////////
//
// Schlick Material
//
///////////////////
