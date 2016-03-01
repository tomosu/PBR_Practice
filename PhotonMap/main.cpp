#include <string>
#include <iostream>
#include <omp.h>
#include "PhotonMap.h"
#include "Camera.h"
#include "Scene.h"
#include "Image.h"
#include "Light.h"
#include "PhotonInteraction.h"


int main()
{
  Real tMax =999999.9;
  Real tMin =0.000001;
  
  // Create scene
  std::string dummy("test");
  //SharedScenePtr cornell =Scene::CreateScene(CORNELL_BOX, dummy);
  SharedScenePtr cornell =Scene::CreateScene(BUNNY_BOX, dummy);
  std::cout <<"Create Scene."<<std::endl;

  
  // Create light
  LightParam lightParam(BALL_SOURCE,
			Vec3(12.0, 12.0, 12.0), //color
			Vec3(0.0, 65.0, 80.0), //center
			2.0, //radius
			0.0, Vec3(0.0, 0.0, 0.0));

  SharedLightPtr light =Light::CreateLight(lightParam);

  
  // Create camera
  Vec3 eye(0.0, 0.0, 50.0);
  Vec3 cameraNormal(0.0, 1.0, 0.0);
  Vec3 cameraScreenCenter(0.0, 20.0, 50.0);

  int cameraSizeU =1280;
  int cameraSizeV =960;
  //int cameraSizeU =640;
  //int cameraSizeV =480;
  //int cameraSizeU =320;
  //int cameraSizeV =240;
  
  Real cameraPitchU =40.0/ (Real)cameraSizeU;
  Real cameraPitchV =30.0/ (Real)cameraSizeV;
  Vec3 cameraUp(0.0, 0.0, 1.0);
  CameraParam param(eye, cameraNormal, cameraUp, cameraScreenCenter,
		    cameraPitchU, cameraPitchV, cameraSizeU, cameraSizeV);

  SharedCameraPtr camera =Camera::CreateCamera(param);

  
  // random
  UniformRandomGenerator01 random(111); 


  // Create photonmap
  PhotonMap photonMap(light, cornell);
  photonMap.ShootPhotons(100000000);
  //photonMap.ShootPhotons(2000000);
  //photonMap.ShootPhotons(800000);
  
  std::cout << "Create PhotonMap" << std::endl;
  std::vector<Vec3> pixel;
  pixel.resize(cameraSizeU *cameraSizeV);

#pragma omp parallel for
  for(int v=0; v< cameraSizeV; v++){    
    std::cout << "Rendering:" << v << "/" << cameraSizeV << std::endl;
    for(int u=0; u< cameraSizeU; u++){
      
      camera->SetImageValue(u, v, Vec3(0.0, 0.0, 0.0));
      Ray ray0 = camera->GeneratePixelCenterRay(u, v);
      //Vec3 pixelColor =photonMap.GetRadiance(random, ray0, 0, 20.0, 1000);
      Vec3 pixelColor =photonMap.GetRadiance(random, ray0, 0, 20.0, 4000);
      pixel[u +cameraSizeU*v] =pixelColor;
    }
  }

  for(int i=0; i<pixel.size(); i++){
    camera->SetImageValue(i%cameraSizeU, i/cameraSizeU, pixel[i]*4.0);
  }

  camera->Binning(2,2);
  camera->ExportPPM("dragon_buddha.ppm");
  
  return 0;

}
