#include "BBox.h"
#include <float.h> 
#include <algorithm>
#include <iostream>

// bool BBox::RayIntersect(const Ray &ray, const Real tMax, const Real tMin)
// {
//   Real intervalMin =tMin;
//   Real intervalMax =tMax;
//   Real p0, p1, t0, t1;

    
//   //x
//   if(ray.Direction.x > 0){
//     p0 =pMin.x;
//     p1 =pMax.x;
//   }else{
//     p0 =pMax.x;
//     p1 =pMin.x;
//   }
//   t0 =(p0 -ray.Origin.x) / ray.Direction.x;
//   t1 =(p1 -ray.Origin.x) / ray.Direction.x;
//   intervalMin = std::max(t0, intervalMin);
//   intervalMax = std::min(t1, intervalMax);
//   if(intervalMin > intervalMax){ return false; }

  
//   //y
//   if(ray.Direction.y > 0){
//     p0 =pMin.y;
//     p1 =pMax.y;
//   }else{
//     p0 =pMax.y;
//     p1 =pMin.y;
//   }
//   t0 =(p0 -ray.Origin.y) / ray.Direction.y;
//   t1 =(p1 -ray.Origin.y) / ray.Direction.y;
//   intervalMin = std::max(t0, intervalMin);
//   intervalMax = std::min(t1, intervalMax);
//   if(intervalMin > intervalMax) return false;

  
//   //z
//   if(ray.Direction.z > 0){
//     p0 =pMin.z;
//     p1 =pMax.z;
//   }else{
//     p0 =pMax.z;
//     p1 =pMin.z;
//   }
//   t0 =(p0 -ray.Origin.z) / ray.Direction.z;
//   t1 =(p1 -ray.Origin.z) / ray.Direction.z;
//   intervalMin = std::max(t0, intervalMin);
//   intervalMax = std::min(t1, intervalMax);
//   return !(intervalMin > intervalMax);
// }


bool BBox::RayIntersect(const Ray &ray, const Real tMax, const Real tMin)
{
  Real t0 =tMin;
  Real t1 =tMax;
    
  //x
  {
    Real invRayDir = 1.0/ray.Direction.x;
    Real tNear = (pMin.x - ray.Origin.x) *invRayDir;
    Real tFar = (pMax.x - ray.Origin.x) *invRayDir;
    
    if(tNear > tFar) std::swap(tNear, tFar);
    t0 = tNear > t0 ? tNear : t0;
    t1 = tFar > t1 ? tFar : t1;
    if(t0 > t1) return false;
  }

  //y
  {
    Real invRayDir = 1.0/ray.Direction.x;
    Real tNear = (pMin.x - ray.Origin.x) *invRayDir;
    Real tFar = (pMax.x - ray.Origin.x) *invRayDir;
    
    if(tNear > tFar) std::swap(tNear, tFar);
    t0 = tNear > t0 ? tNear : t0;
    t1 = tFar > t1 ? tFar : t1;
    if(t0 > t1) return false;
  }

  //z
  {
    Real invRayDir = 1.0/ray.Direction.x;
    Real tNear = (pMin.x - ray.Origin.x) *invRayDir;
    Real tFar = (pMax.x - ray.Origin.x) *invRayDir;
    
    if(tNear > tFar) std::swap(tNear, tFar);
    t0 = tNear > t0 ? tNear : t0;
    t1 = tFar > t1 ? tFar : t1;
    return !(t0 > t1);
  }
}


bool BBox::IsInside(const Ray &ray)
{
  return (pMin.x <= ray.Origin.x & ray.Origin.x <= pMax.x) &
    (pMin.y <= ray.Origin.y & ray.Origin.y <= pMax.y) &
    (pMin.z <= ray.Origin.z & ray.Origin.z <= pMax.z);
}
