#include <iostream>
#include "Light.h"

int main(){
  UniformRandomGenerator01 random(100);

  LightParam param(BALL_SOURCE,
		   Vec3(1.0, 1.0, 1.0), //color
		   Vec3(0.0, 0.0, 0.0), //center
		   1.5,//Radius
		   0.0,
		   0.0);

  SharedLightPtr light =Light::CreateLight(param);
  for(int i=0; i<15; i++){
    Ray ray =light->GenerateRandomRay(random);
    std::cout << "pos(" << ray.Origin.x << "," << ray.Origin.y << "," << ray.Origin.z << ")" << std::endl;
    std::cout << "dir(" << ray.Direction.x << "," << ray.Direction.y << "," << ray.Direction.z << ")" << std::endl;
  }
  return 0;
}
