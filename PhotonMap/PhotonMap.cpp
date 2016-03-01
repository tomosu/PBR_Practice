#include <algorithm>
#include <stdio.h>
#include "RandomNumberGenerator.h"
#include "PhotonMap.h"
#include "PhotonInteraction.h"

void PrintVec3(std::string title, Vec3& v){
  std::cout << title << v.x << " " << v.y << " " << v.z <<std::endl;
}

void PhotonMap::ShootPhotons(const int photonNum)
{
  Real tMax=999999.9, tMin=0.00001;
  UniformRandomGenerator01 random(photonNum);

  
  /////////////////
  // iteration start
  /////////////////
  int photonCount =0;
  int reflectCount =0;
  for(int i=0; i<photonNum; i++){

    if(i%5000 ==0){std::cout << "photon num:" << i << std::endl;}
      
    ///////////////////
    // get hit information
    ///////////////////
    Ray ray =this->Light->GenerateRandomRay(random);
    Real lightRadius2 =this->Light->GetRadius() *this->Light->GetRadius();
    Real fluxRate =4.0 *M_PI *M_PI *lightRadius2 /(Real)photonNum;
    Vec3 flux =this->Light->GetColor() *fluxRate;


    ///////////////////
    // tracing photon
    ///////////////////
    bool isTracing =true;
    while(isTracing){
      
      ///////////////
      // 0 flux
      ///////////////
      if (std::max(flux.x, std::max(flux.y, flux.z)) <= 0.0) break;
      
      ///////////////////
      // does hit?
      ///////////////////
      HitInformation info;
      this->Scene->GetHitInformation(ray, tMax, tMin, &info);
      if(!info.DoesHit)break;
      
      /////////////////
      // add photon to map
      /////////////////
      if(info.MaterialPtr->GetReflectionType() == DIFFUSE){
	photonCount++;
	Vec3 hitPos =ray.Origin +ray.Direction *info.RayParam;
	this->Photons.AddElement(Photon(hitPos, ray.Direction, flux));
      }
	
      ////////////////
      // update ray & flux
      ////////////////
      if(DoesReflect(random, 0, info)){
	reflectCount++;
	ray =UpdateRay(random, ray, info);
	flux =flux.Multiply(info.MaterialPtr->GetColor()) /info.MaterialPtr->GetReflectance();
      }else{
	isTracing =false;
	continue;
      }

    }//tracing end
  }//iteration end


  //////////////
  // build tree
  //////////////
  std::cout << "photon count:" << photonCount << " reflect count:" << reflectCount << std::endl;
  this->Photons.BuildTree();
}




Vec3 PhotonMap::GetRadiance(UniformRandomGenerator01 &random, const Ray &ray, const int depth,
			    const Real searchRadius, const int searchNum)
{
  
  Real tMax=9999999.9, tMin=0.00001;
  ///////////////////
  // does hit?
  ///////////////////
  HitInformation info;
  this->Scene->GetHitInformation(ray, tMax, tMin, &info);
  if(!info.DoesHit){ return Vec3(0.0, 0.0, 0.0); }


  ///////////////////
  // Russian Rourette
  ///////////////////
  Real RRProbability =info.MaterialPtr->GetReflectance();
  if(depth > 30){
    if(random.GenRand() > RRProbability ){
      std::cout << "depth_over" << std::endl;
      return Vec3(0.0, 0.0, 0.0);
    }
  }else{
    RRProbability =1.0;
  }

  
  //////////////////
  // calc radiance
  //////////////////
  switch(info.MaterialPtr->GetReflectionType()){

    //////////////////
    // Diffuse(use photon map)
    //////////////////
  case DIFFUSE:{
    Vec3 hitPos =ray.Origin +ray.Direction *info.RayParam;
    PhotonMapTree::GatheredElementsQueue result;
    Query query(searchRadius, searchNum, hitPos);
    this->Photons.GatherElements(query, &result);
    
    if(result.size() ==0){std::cout << "! empty photon map" << std::endl; return Vec3(0.0, 0.0, 0.0);}
    
    PhotonMapTree::TaggedElement firstElem;
    firstElem =result.top();
    const Real maxDistance2 =firstElem.Distance2;
    const Real maxDistance =sqrt(maxDistance2);
    const Real k=1.1;
    Vec3 totalFlux(0.0, 0.0, 0.0);

    // use cone filter
    while(!result.empty()){
      PhotonMapTree::TaggedElement elem =result.top();
      result.pop();
      const Real weight = 1.0 - (sqrt(elem.Distance2) / (k * maxDistance));
      const Vec3 tmpFlux = info.MaterialPtr->GetColor().Multiply(elem.ElementPtr->Color) / M_PI;
      totalFlux = totalFlux + tmpFlux *weight;
    }
    
    totalFlux = totalFlux / (1.0 - 2.0 / (3.0 * k)); //normalization
    if (maxDistance2 > 0.0) {
      if(std::isnan(totalFlux.x)){std::cout<< "nan error @diffuse" << std::endl; exit(0);}
      return totalFlux / (M_PI * maxDistance2) / RRProbability;
    }
    break;
  }
   
 
    //////////////////
    // Specular(ray trace)
    //////////////////
  case SPECULAR:{
    Vec3 reflectRadiance =GetRadiance(random, UpdateRaySpecular(ray, info),
				      depth+1, searchRadius, searchNum);
    return info.MaterialPtr->GetColor().Multiply(reflectRadiance) / RRProbability;;
    break;
  }


    //////////////////
    // Refraction(ray trace)
    //////////////////
  case REFRACTION:{

    Vec3 normal = info.NormalVector;
    Vec3 orientNormal = (ray.Direction.Dot(normal) < 0.0) ? normal : normal *(-1.0);      
    bool isIncidence = normal.Dot(orientNormal) > 0.0;
    Real eta0 =1.0; //vacuum's refractive
    Real eta =info.MaterialPtr->GetRefractive();
    Real etaRatio =isIncidence ? eta0/eta : eta/eta0;
    Real cos_t =orientNormal.Dot(ray.Direction);
    Real cos_2t = 1.0 -etaRatio *etaRatio *(1.0 -cos_t *cos_t);
	  
    if(cos_2t < 0.0){ //reflection
      Vec3 reflectRadiance =GetRadiance(random, UpdateRaySpecular(ray, info),
					depth+1, searchRadius, searchNum);
      return info.MaterialPtr->GetColor().Multiply(reflectRadiance) / RRProbability;
    }
    
    Vec3 refractRayDirection =(ray.Direction -orientNormal *cos_t)*etaRatio -orientNormal *sqrt(cos_2t) ;
    refractRayDirection =refractRayDirection.Normalize();

    //Fresnel coef
    Real a =eta -eta0;
    Real b =eta +eta0;
    Real F_real =(a*a)/(b*b);
    Real c =1.0 -(isIncidence ? -cos_t : refractRayDirection.Dot(orientNormal *(-1.0)));
    Real SchlickFr =F_real +(1.0 -F_real) *pow(c, 5.0);
    Real Tr = (1.0-SchlickFr)*(etaRatio *etaRatio);

    if(depth > 2){
      Real probability = 0.1 +0.8 *SchlickFr;

      if (random.GenRand() < probability) { //reflection
      	Vec3 reflectRadiance =GetRadiance(random, UpdateRaySpecular(ray, info),
					  depth+1, searchRadius, searchNum) *SchlickFr;
	return info.MaterialPtr->GetColor().Multiply(reflectRadiance) /probability /RRProbability;
	
      } else { //refaraction
	Vec3 refractRadiance =GetRadiance(random, UpdateRayRefractionIntruder(ray, info),
					  depth+1, searchRadius, searchNum) *Tr;
	return info.MaterialPtr->GetColor().Multiply(refractRadiance) /(1.0-probability) /RRProbability;
      }
	
    }else{ //trace both reflection & refraction
      Vec3 reflectRadiance =GetRadiance(random, UpdateRaySpecular(ray, info),
					depth+1, searchRadius, searchNum);
      Vec3 refractRadiance =GetRadiance(random, UpdateRayRefractionIntruder(ray, info),
					depth+1, searchRadius, searchNum);

      Vec3 totalRadiance =(reflectRadiance *SchlickFr) +(refractRadiance *Tr);

      if(std::isnan(totalRadiance.x)){std::cout<< "nan error" << std::endl; exit(0);}
      return  info.MaterialPtr->GetColor().Multiply(totalRadiance) / RRProbability;
    }
    break;
  }
  

  default:{
    return Vec3(0.0, 0.0, 0.0);
    break;
  }
  }// end switch
}
