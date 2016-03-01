#include <cassert>
#include <iostream>
#include "Shape.h"
#include "MiscCalculation.h"



///////////////////
//
// Shape(interface)
//
///////////////////
SharedShapePtr Shape::CreateShape(SharedMaterialPtr &material, const ShapeParam &param)
{
  switch(param.Type){
    
  case SPHERE:{
    SharedShapePtr ret(new Sphere(param.Center, param.Radius) );
    ret->SetMaterialPtr(material);
    return ret;
    break;
  }
  
  default:{
    assert(!"invalid scene object type\n");
    break;
  } 
  }

}

SharedShapePtr Shape::CreateTriangleFace(SharedMaterialPtr &material,
					 Vec3 &V1, Vec3 &V2, Vec3 &V3,
					 Vec3 &Vn1, Vec3 &Vn2, Vec3 &Vn3){
  SharedShapePtr ret(new TriangleFace(V1, V2, V3, Vn1, Vn2, Vn3) );
  ret->SetMaterialPtr(material);
  return ret;
}

//////////////////////////
//
// Sphere
//
//////////////////////////
inline bool Sphere::GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info)
{
  Vec3 v =this->Center -ray.Origin;
  Vec3 vd =ray.Direction;
  
  //coef of quadratic equation
  Real a =1.0;
  Real b = -2.0 *v.Dot(vd);
  Real c = v.Dot(v) -(this->Radius *this->Radius);
  
  if( Det(a,b,c) >0.0 ){

    Real t =QuadraticEquationRootSmaller(a,b,c);

    if( t < tMin){ t = QuadraticEquationRootBigger(a,b,c); }
    if( t < tMin || t > tMax){ return false; }

    info->DoesHit =true;
    info->RayParam =t;
    info->NormalVector =this->GetNormalVector(ray.Origin +ray.Direction *t);
    info->MaterialPtr =this->GetMaterialPtr();
    return true;
  }
  return false;
}



inline bool Sphere::DoesHit(const Ray &ray, const Real tMax, const Real tMin)
{
  Vec3 v =this->Center -ray.Origin;
  Vec3 vd =ray.Direction;
  
  //coef of quadratic equation
  Real a =1.0;
  Real b = -2.0 *v.Dot(vd);
  Real c = v.Dot(v) -(this->Radius *this->Radius);
  
  if( Det(a,b,c) >=0.0 ){
    Real t =QuadraticEquationRootSmaller(a,b,c);

    if( t < tMin){ t =QuadraticEquationRootBigger(a,b,c); }
    if( t < tMin || t > tMax){ return false; }

    return true;
  }

  return false;
}



inline Vec3 Sphere::GetNormalVector(const Vec3 &hitPoint)
{
  Vec3 nv =hitPoint -this->Center;
  return nv.Normalize();
}


BBox Sphere::BoundingBox()
{
  Vec3 unit(1.0, 1.0, 1.0);
  Vec3 p1 =this->Center +(unit *this->Radius);
  Vec3 p2 =this->Center -(unit *this->Radius);
  return BBox(p1, p2);
}

  

//////////////////////////
//
// TriangleFace
//
//////////////////////////

//Moller-Trumbore method
inline bool TriangleFace::GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info)
{
  Vec3 edge1 =this->V2 -this->V1;
  Vec3 edge2 =this->V3 -this->V1;
  
  Vec3 p =ray.Direction.Cross(edge2);;
  Real det =edge1.Dot(p);

  if(det > -tMin && det < tMin){return false;}
  Real invDet =1.0/det;

  Vec3 t =ray.Origin-this->V1;
  Real u =t.Dot(p)*invDet;

  if(u <0.0 || u >1.0){return false;}

  Vec3 q =t.Cross(edge1);
  Real v =ray.Direction.Dot(q)*invDet;

  if(v < 0.0 || u+v >1.0){return false;}
  
  Real param =edge2.Dot(q)*invDet;

  if(param >tMin && param <tMax){
    info->DoesHit =true;
    info->RayParam =param;
    info->NormalVector =this->GetNormalVector(ray.Origin +ray.Direction *param);
    info->MaterialPtr =this->GetMaterialPtr();
    return true;
  }
  
  return false;
}


inline bool TriangleFace::DoesHit(const Ray &ray, const Real tMax, const Real tMin)
{
  Vec3 edge1 =this->V2 -this->V1;
  Vec3 edge2 =this->V3 -this->V1;
  
  Vec3 p =ray.Direction.Cross(edge2);;
  Real det =edge1.Dot(p);

  if(det > -tMin && det < tMin){return false;}
  Real invDet =1.0/det;

  Vec3 t =ray.Origin-this->V1;
  Real u =t.Dot(p)*invDet;

  if(u <0.0 || u >1.0){return false;}

  Vec3 q =t.Cross(edge1);
  Real v =ray.Direction.Dot(q)*invDet;

  if(v < 0.0 || u+v >1.0){return false;}
  
  Real param =edge2.Dot(q)*invDet;

  if(param >tMin && param <tMax){
    return true;
  }
  
  return false;
}


inline Vec3 TriangleFace::GetNormalVector(const Vec3 &hitPoint)
{
  return (this->Vn1 +this->Vn2 +this->Vn3).Normalize();
}


BBox TriangleFace::BoundingBox()
{
  return BBox(this->V1, this->V2, this->V3);
}
