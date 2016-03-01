#include <memory>
#include <vector>
#include <cassert>
#include <float.h>
#include <iostream>

#include "Shape.h"
#include "Bvh.h"


////////////////////
// Debug func
////////////////////
void ShapeDebug(SharedShapePtr &in)
{
  BBox bbox =in->BoundingBox();
  Real rad =(bbox.Max()-bbox.Min()).x/2.0;
  Vec3 center =(bbox.Max() +bbox.Min())/2.0;
  std::cout << " cx:" << center.x << " cy:" << center.y << " cz:" << center.z << std::endl;
  std::cout << " rad:" << rad << std::endl;
}

void BoxDebug(SharedShapePtr &in)
{
  BBox bbox =in->BoundingBox();
  std::cout << " minx:" << bbox.Min().x << " miny:" << bbox.Min().y << " minz:" << bbox.Min().z << std::endl;
  std::cout << " maxx:" << bbox.Max().x << " maxy:" << bbox.Max().y << " maxz:" << bbox.Max().z << std::endl;
}


/////////////////////
// Compare obj
/////////////////////
class LessCentroidX
{
public:
  bool operator()(SharedShapePtr const &left, SharedShapePtr const &right){
    Real l=left->BoundingBox().Max().x + left->BoundingBox().Min().x;
    Real r=right->BoundingBox().Max().x + right->BoundingBox().Min().x; 
    return l < r; 
  }
};


class LessCentroidY
{
public:
  bool operator()(SharedShapePtr const &left, SharedShapePtr const &right){
    Real l=left->BoundingBox().Max().y + left->BoundingBox().Min().y;
    Real r=right->BoundingBox().Max().y + right->BoundingBox().Min().y; 
    return l < r; 
  }
};


class LessCentroidZ
{
public:
  bool operator()(SharedShapePtr const &left, SharedShapePtr const &right){
    Real l=left->BoundingBox().Max().z + left->BoundingBox().Min().z;
    Real r=right->BoundingBox().Max().z + right->BoundingBox().Min().z; 
    return l < r; 
  }
};



//////////////////////////
//
// BVH
//
//////////////////////////
Bvh::Bvh(SharedShapePtr& s1, SharedShapePtr& s2)
{
  this->Left =s1;
  this->Right =s2;
  this->Bbox =Surround(this->Left->BoundingBox(), this->Right->BoundingBox());  
};
  

Bvh::Bvh(SharedShapePtr& s1, SharedShapePtr& s2, BBox &bbox_in)
{
  this->Left =s1;
  this->Right =s2;
  this->Bbox =bbox_in;
};

  
ShapePtrVectorIterator Qsplit(ShapePtrVectorIterator begin, ShapePtrVectorIterator end, Real pivot, int depth)
{

  enum Axis{
    X=0,
    Y=1,
    Z=2
  };

  int axis =depth %3;
  switch (axis) {
  case X: std::sort(begin, end, LessCentroidX()); break;
  case Y: std::sort(begin, end, LessCentroidY()); break;
  case Z: std::sort(begin, end, LessCentroidZ()); break;
  }

  ShapePtrVectorIterator mid = begin;
  int shapeCount=0;
  
  for(ShapePtrVectorIterator it =begin; it !=end; it++){
    shapeCount++;
    BBox bbox = it->get()->BoundingBox();
    Real centroid;
    switch (axis) {
    case X: centroid = (bbox.Min().x +bbox.Max().x)/2.0; break;
    case Y: centroid = (bbox.Min().y +bbox.Max().y)/2.0; break;
    case Z: centroid = (bbox.Min().z +bbox.Max().z)/2.0; break;
    }
    if(centroid < pivot){
      mid++;
    }
  }

  if(mid == begin || mid == end-1){
    mid =begin;
    for(int i=0; i<shapeCount/2; i++) {mid++;}
  }
  
  return mid;
}



Bvh::Bvh(std::vector<SharedShapePtr> &objects)
{
  std::sort(objects.begin(), objects.end(), LessCentroidX());

  ShapePtrVectorIterator begin =objects.begin();
  ShapePtrVectorIterator end =objects.end();
  ShapePtrVectorIterator last =objects.end();
  
  if( objects.size() == 1) { *this =Bvh(*begin, *begin); return; }
  if( objects.size() == 2) { *this =Bvh(*begin, *(begin+1)); return; }

  this->Bbox = begin->get()->BoundingBox();
  for(auto it =begin; it !=end; it++){
    this->Bbox =Surround(this->Bbox, it->get()->BoundingBox() );
  }
 
  Vec3 pivot =(this->Bbox.Max() + this->Bbox.Min()) /2.0;
  ShapePtrVectorIterator mid =Qsplit(begin, end, pivot.x, 0);

  this->Left = this->BuildBranch(begin, mid, last, 1);
  this->Right = this->BuildBranch(mid, end, last, 1); 
}


SharedShapePtr Bvh::BuildBranch(ShapePtrVectorIterator begin, ShapePtrVectorIterator end, ShapePtrVectorIterator last, int depth)
{
  enum Axis{
    X=0,
    Y=1,
    Z=2
  };

  if( end -begin == 1) {
    SharedShapePtr ret(new Bvh(*begin, *begin));
    return ret;
  }
  
  if( end -begin == 2) {
    SharedShapePtr ret(new Bvh(*begin, *(end-1)));
    return ret;
  }

  BBox box = begin->get()->BoundingBox();
  for(auto it =begin; it!=end; it++){
    box =Surround(box, it->get()->BoundingBox() );
  }
  
  int axis =depth %3;
  Real pivot;
  switch (axis) {
  case X: pivot = (box.Min().x +box.Max().x)/2.0; break;
  case Y: pivot = (box.Min().y +box.Max().y)/2.0; break;
  case Z: pivot = (box.Min().z +box.Max().z)/2.0; break;
  }

  ShapePtrVectorIterator mid =Qsplit(begin, end, pivot, depth);

  if(mid+1 == end && end == last){
    assert(!"Bvh construction error.");
  }

  SharedShapePtr left= this->BuildBranch(begin, mid, last, depth+1);
  SharedShapePtr right= this->BuildBranch(mid, end, last, 1);
  
  SharedShapePtr ret(new Bvh( left, right, box));
  return ret;
}


inline bool Bvh::GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation *info)
{
  if( !( (this->Bbox.RayIntersect(ray, tMax, tMin)) )  ){ return false; }

  bool DoesHitL =false;
  bool DoesHitR =false;
  info->RayParam =tMax;
  
  DoesHitL =this->Left->GetHitInformation(ray, tMax, tMin, info);

  Real tmp =info->RayParam;
  DoesHitR =this->Right->GetHitInformation(ray, tmp, tMin, info);

  return (DoesHitL || DoesHitR);
}



inline bool Bvh::DoesHit(const Ray &ray, const Real tMax, const Real tMin)
{
  if( !(this->Bbox.RayIntersect(ray, tMax, tMin)) ){ return false; }

  if(this->Right->DoesHit(ray, tMax, tMin)){ return true;}
  return this->Left->DoesHit(ray, tMax, tMin);
}



inline Vec3 Bvh::GetNormalVector(const Vec3 &hitPoint)
{
  assert(!"not implemented");
  return hitPoint;
}
