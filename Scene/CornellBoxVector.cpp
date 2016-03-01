#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "CornellBox.h"
#include "Ray.h"
#include "Scene.h"
#include "Shape.h"


//////////////////////////////
//
// Cornel box scene for test
//
//////////////////////////////
void CornellBox::BuildScene()
{
  // Left Wall
  MaterialParam materialParam1( DIFFUSE, Vec3(0.75, 0.25, 0.25), 0.0, 0.0 );
  SharedMaterialPtr redDiffuseMaterial =Material::CreateMaterial(materialParam1);
  ShapeParam leftWallParam( Vec3(-1e5-50, 50, 100), 1e5 );
  Shapes.push_back( Shape::CreateShape( redDiffuseMaterial, leftWallParam ) );

  // Right Wall
  MaterialParam materialParam2( DIFFUSE, Vec3(0.25, 0.25, 0.75), 0.0, 0.0 );
  SharedMaterialPtr blueDiffuseMaterial =Material::CreateMaterial(materialParam2);
  ShapeParam rightWallParam( Vec3(1e5+50, 50, 100), 1e5 );
  Shapes.push_back( Shape::CreateShape( blueDiffuseMaterial, rightWallParam ) );

  // Back Wall
  MaterialParam materialParam3( DIFFUSE, Vec3(0.25, 0.25, 0.25), 0.0, 0.0 );
  SharedMaterialPtr grayDiffuseMaterial =Material::CreateMaterial(materialParam3);
  ShapeParam backWallParam( Vec3(0, 1e5+200, 50), 1e5 );
  Shapes.push_back( Shape::CreateShape( grayDiffuseMaterial, backWallParam ) );

  // Front Wall
  MaterialParam materialParam4( DIFFUSE, Vec3(0.0, 0.0, 0.0), 0.0, 0.0 );
  SharedMaterialPtr blackDiffuseMaterial =Material::CreateMaterial(materialParam4);
  ShapeParam frontWallParam( Vec3(0, -1e5-50, 50), 1e5 );
  Shapes.push_back( Shape::CreateShape( blackDiffuseMaterial, frontWallParam ) );

  // Floor
  ShapeParam floorParam( Vec3(0, 0, -1e5), 1e5 );
  Shapes.push_back( Shape::CreateShape( grayDiffuseMaterial, floorParam ) );

  // Ceil
  ShapeParam ceilParam( Vec3(0, 0, 1e5+100), 1e5 );
  Shapes.push_back( Shape::CreateShape( grayDiffuseMaterial, ceilParam ) );
    
  //Green Ball
  MaterialParam materialParam6( DIFFUSE, Vec3(0.25, 0.75, 0.25), 0.0, 0.0 );
  SharedMaterialPtr greenDiffuseMaterial =Material::CreateMaterial(materialParam6);
  ShapeParam ballParam( Vec3(-20, 80, 20), 10 );
  Shapes.push_back( Shape::CreateShape( greenDiffuseMaterial, ballParam ) );

  //White Ball
  MaterialParam materialParam7( DIFFUSE, Vec3(0.8, 0.8, 0.8), 0.0, 0.0 );
  SharedMaterialPtr whiteDiffuseMaterial =Material::CreateMaterial(materialParam7);
  ShapeParam ballParam2( Vec3(30, 100, 60), 11 );
  Shapes.push_back( Shape::CreateShape( whiteDiffuseMaterial, ballParam2 ) );

}



bool CornellBox::GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info)
{
  Real tmpMax =tMax;

  for(auto& obj : this->Shapes){
    if(obj->GetHitInformation(ray, tmpMax, tMin, info) ){
      tmpMax = info->RayParam;
    }
  }
}



Ray CornellBox::CalcNextRay(const Ray &ray, const Real rayParam, const SharedShapePtr &obj)
{
  assert(!"not implemented");
  return ray;
}
