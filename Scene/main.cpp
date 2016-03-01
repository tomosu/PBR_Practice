#include <string>
#include <iostream>

#include "Scene.h"
#include "Camera.h"
#include "Image.h"


int main()
{

  // Create Scene
  std::string dummy("test");
  //SharedScenePtr cornell =Scene::CreateScene(CORNELL_BOX, dummy);
  SharedScenePtr cornell =Scene::CreateScene(BUNNY_BOX, dummy);
  std::cout <<"Create Scene."<<std::endl;


  // Simple Test
  std::cout << " one shot test" <<std::endl;
  Vec3 eye(0.0, 0.0, 50.0);
  Vec3 cameraNormal(0.0, 1.0, 0.0);
  Ray ray0(eye, cameraNormal);

  Real tMax =999999.9;
  Real tMin =0.00001;
  HitInformation testinfo;

  cornell->GetHitInformation(ray0, tMax, tMin, &testinfo);

  if(testinfo.DoesHit){ std::cout << "hit" << std::endl;}
  

  // Ray trace
  std::cout <<"Ray trace test."<<std::endl;
  Vec3 cameraScreenCenter(0.0, 20.0, 50.0);

  int cameraSizeU =320;
  int cameraSizeV =240;
  //int cameraSizeU =640;
  //int cameraSizeV =480;
  //int cameraSizeU =2560;
  //int cameraSizeV =1920;

  Real cameraPitchU =40.0/ (Real)cameraSizeU;
  Real cameraPitchV =30.0/ (Real)cameraSizeV;
  
  Vec3 cameraUp(0.0, 0.0, 1.0);
  CameraParam param(eye, cameraNormal, cameraUp, cameraScreenCenter,
		    cameraPitchU, cameraPitchV, cameraSizeU, cameraSizeV);
  
  SharedCameraPtr camera =Camera::CreateCamera(param);

  Vec3 light(0.0, 80.0, 80.0);
  for(int u=0; u< camera->GetSizeU(); u++){
    std::cout << "rendering now" <<std::endl;
    for(int v=0; v< camera->GetSizeV(); v++){

      // initial value
      camera->SetImageValue(u, v, 0.0);

      // hit point check
      HitInformation info;
      Ray ray = camera->GeneratePixelCenterRay(u, v);
      cornell->GetHitInformation(ray, tMax, tMin, &info);
      
      // skip
      if(!info.DoesHit) continue;
      
      // trace to diffuse point
      int ReflectionCount =0;
      while( !(info.DoesHit && info.MaterialPtr->GetReflectionType() == DIFFUSE ) ) {

	ReflectionCount++;
	if(ReflectionCount >15) break;

	// SPECULATION or REFRACTION
	switch(info.MaterialPtr->GetReflectionType() ){

	case SPECULAR:{
	  Vec3 tmpHitPos =ray.Origin +ray.Direction *(info.RayParam *0.999);
	  Vec3 tmpNormal =info.NormalVector;
	  Vec3 ReflectRayDirection =ray.Direction -tmpNormal *2.0 *tmpNormal.Dot(ray.Direction);
	  ray =Ray(tmpHitPos, ReflectRayDirection.Normalize() );
	  cornell->GetHitInformation(ray, tMax, tMin, &info);
	  break;
	}

	case REFRACTION:{
	  Vec3 tmpNormal =info.NormalVector;
	  Vec3 orientNormal =tmpNormal.Dot(ray.Direction) < 0.0 ? tmpNormal : tmpNormal*(-1.0) ;
	  bool isIncidence = tmpNormal.Dot(orientNormal) > 0.0;
	  Real nc =1.0; //vacuum
	  Real nt =info.MaterialPtr->GetRefractive();
	  Real nnt =isIncidence ? nc/nt : nt/nc;
	  Real ddn =orientNormal.Dot(ray.Direction);
	  Real cos2t = 1.0 -nnt *nnt *(1.0 -ddn *ddn);
	  
	  Vec3 tmpHitPos =ray.Origin +ray.Direction *(info.RayParam *1.001);
	  Vec3 ReflectRayDirection =ray.Direction -tmpNormal *2.0 *tmpNormal.Dot(ray.Direction);
	  if(cos2t < 0.0){ //reflection
	    ray =Ray(tmpHitPos, ReflectRayDirection.Normalize() );
	    cornell->GetHitInformation(ray, tMax, tMin, &info);
	  }else{
	    Vec3 RefractRayDirection =(ray.Direction *nnt -tmpNormal *(isIncidence ? 1.0 : -1.0) *(ddn *nnt +sqrt(cos2t))).Normalize();
	    
	    //Fresnel coef
	    Real a =nt-nc;
	    Real b =nt+nc;
	    Real R0 =(a*a)/(b*b);
	    Real c =1.0 -(isIncidence ? ddn : RefractRayDirection.Dot(tmpNormal));
	    Real Re =R0 +(1.0 -R0) *pow(c, 5.0);
	    Real Tr =1.0 -Re;
	    Real probability = 0.25 +0.5 *Re;

	    //Refractive
	    ray =Ray(tmpHitPos, RefractRayDirection);
	    cornell->GetHitInformation(ray, tMax, tMin, &info);
	  }
	  
	}

	default:{
	  break;
	}
	  
	}
      }
      
      // hit point info
      Vec3 hitPos =ray.Origin +ray.Direction *info.RayParam;
      Vec3 color =info.MaterialPtr->GetColor();

      // get shadow info
      HitInformation shadowInfo;
      Ray shadowRay(hitPos, (light -hitPos).Normalize());
      cornell->GetHitInformation(shadowRay, (light-hitPos).Length()-0.01, 0.1, &shadowInfo);
      
      // shadow effect
      if(shadowInfo.DoesHit == true){
	color = Vec3(0.0, 0.0, 0.0)*0.75 + color*0.25;
      }
      
      // rendering
      Vec3 normal =info.NormalVector;
      Vec3 orientNormal = (ray.Direction.Dot(normal) >0.0) ? normal : normal *(-1.0);
      Vec3 lightDirection =hitPos-light;
      Real ratio =orientNormal.Dot(lightDirection.Normalize());
      camera->SetImageValue(u, v, color *ratio);
    }
  }

  camera->Binning(2,2);
  camera->ExportPPM("image.ppm");
  
  return 0;
}
