#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "CornellBox.h"
#include "Ray.h"
#include "Scene.h"
#include "Shape.h"
#include "Bvh.h"

//////////////////////////////
//
// Cornel box scene for test
//
//////////////////////////////
void CornellBox::BuildScene()
{

  std::vector<SharedShapePtr> shapes;
  
  // Left Wall
  MaterialParam materialParam1( DIFFUSE, Vec3(0.75, 0.25, 0.25), 0.75, 0.0 );
  SharedMaterialPtr redDiffuseMaterial =Material::CreateMaterial(materialParam1);
  ShapeParam leftWallParam( Vec3(-1e5-100, 50, 100), 1e5 );
  shapes.push_back( Shape::CreateShape( redDiffuseMaterial, leftWallParam ) );

  // Right Wall
  MaterialParam materialParam2( DIFFUSE, Vec3(0.25, 0.25, 0.75), 0.75, 0.0 );
  SharedMaterialPtr blueDiffuseMaterial =Material::CreateMaterial(materialParam2);
  ShapeParam rightWallParam( Vec3(1e5+100, 50, 100), 1e5 );
  shapes.push_back( Shape::CreateShape( blueDiffuseMaterial, rightWallParam ) );

  // Back Wall
  MaterialParam materialParam3( DIFFUSE, Vec3(0.25, 0.25, 0.25), 0.25, 0.0 );
  SharedMaterialPtr grayDiffuseMaterial =Material::CreateMaterial(materialParam3);
  ShapeParam backWallParam( Vec3(0, 1e5+200, 50), 1e5 );
  shapes.push_back( Shape::CreateShape( grayDiffuseMaterial, backWallParam ) );

  // Front Wall
  MaterialParam materialParam4( DIFFUSE, Vec3(0.0, 0.0, 0.0), 0.0, 0.0 );
  SharedMaterialPtr blackDiffuseMaterial =Material::CreateMaterial(materialParam4);
  ShapeParam frontWallParam( Vec3(0, -1e5-150, 50), 1e5 );
  shapes.push_back( Shape::CreateShape( blackDiffuseMaterial, frontWallParam ) );

  // Floor
  ShapeParam floorParam( Vec3(0, 0, -1e5), 1e5 );
  shapes.push_back( Shape::CreateShape( grayDiffuseMaterial, floorParam ) );

  // Ceil
  ShapeParam ceilParam( Vec3(0, 0, 1e5+100), 1e5 );
  shapes.push_back( Shape::CreateShape( grayDiffuseMaterial, ceilParam ) );
    
  //Green Ball
  MaterialParam materialParam6( DIFFUSE, Vec3(0.25, 0.75, 0.25), 0.75, 0.0 );
  SharedMaterialPtr greenDiffuseMaterial =Material::CreateMaterial(materialParam6);
  ShapeParam ballParam( Vec3(-24, 80, 14), 12 );
  shapes.push_back( Shape::CreateShape( greenDiffuseMaterial, ballParam ) );

  //White Ball
  MaterialParam materialParam7( DIFFUSE, Vec3(0.8, 0.8, 0.8), 0.8, 0.0 );
  SharedMaterialPtr whiteDiffuseMaterial =Material::CreateMaterial(materialParam7);
  ShapeParam ballParam2( Vec3(30, 90, 60), 13 );
  shapes.push_back( Shape::CreateShape( whiteDiffuseMaterial, ballParam2 ) );


  //Specular Ball
  MaterialParam materialParam8( SPECULAR, Vec3(0.99, 0.99, 0.99), 0.99, 0.0 );
  SharedMaterialPtr specDiffuseMaterial =Material::CreateMaterial(materialParam8);
  ShapeParam ballParam3( Vec3(-18, 90, 40), 13 );
  shapes.push_back( Shape::CreateShape( specDiffuseMaterial, ballParam3 ) );

  
  //Refractive Ball
  MaterialParam materialParam9( REFRACTION, Vec3(0.99, 0.99, 0.99), 0.99, 1.6 );
  SharedMaterialPtr refracDiffuseMaterial =Material::CreateMaterial(materialParam9);
  ShapeParam ballParam4( Vec3(15, 75, 22), 13 );
  shapes.push_back( Shape::CreateShape( refracDiffuseMaterial, ballParam4 ) );

  
  //LBlue Ball
  MaterialParam materialParam10( DIFFUSE, Vec3(0.2, 0.8, 0.8), 0.8, 0.0 );
  SharedMaterialPtr lblueDiffuseMaterial =Material::CreateMaterial(materialParam10);
  ShapeParam ballParam5( Vec3(-30, 60, 60), 7 );
  shapes.push_back( Shape::CreateShape( lblueDiffuseMaterial, ballParam5 ) );

  
  //BuildBVH
  SharedShapePtr bvh(new Bvh(shapes));
  this->ShapesBVH =bvh;
}



bool CornellBox::GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info)
{
  return this->ShapesBVH->GetHitInformation(ray, tMax, tMin, info);
}



Ray CornellBox::CalcNextRay(const Ray &ray, const Real rayParam, const SharedShapePtr &obj)
{
  assert(!"not implemented");
  return ray;
}
