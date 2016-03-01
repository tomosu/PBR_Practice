#include <iostream>
#include <cmath>

#include "Material.h"

int main()
{
  MaterialParam simpleParam(DIFFUSE, Vec3(0.5, 0.0, 0.0), 0.0, 0.0);
  SharedMaterialPtr simpleMaterial =Material::CreateMaterial(simpleParam);
  
  return 0;
}
