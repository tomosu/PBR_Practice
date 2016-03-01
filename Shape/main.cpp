#include <iostream>
#include <cmath>

#include "HitInformation.h"
#include "Shape.h"

int main()
{

  SharedMaterialPtr material;
  //////////////////////
  // create sphere
  //////////////////////
  Vec3 center(0.0, 0.0, 0.0);
  Real radius =3.0;

  ShapeParam param(center, radius);
  SharedShapePtr sphere =Shape::CreateShape(material, param);


  //////////////////////
  // get hit point
  //////////////////////
  Vec3 origin(-10.0, 0.0, 0.0);
  Vec3 direction(1.0, 0.0, 0.0);
  Ray ray(origin, direction);
  HitInformation info;
  
  sphere->GetHitInformation(ray, 9999999.9, 0.00001, &info);
  std::cout << "t:" << info.RayParam << std::endl; 
  
  return 0;
}
