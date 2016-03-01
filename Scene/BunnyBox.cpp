#include <cmath>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "BunnyBox.h"
#include "Ray.h"
#include "Scene.h"
#include "Shape.h"
#include "Bvh.h"


//////////////////////////////
//
// Cornel box scene for test
//
//////////////////////////////
void BunnyBox::BuildScene()
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
  MaterialParam materialParam4( DIFFUSE, Vec3(0.25, 0.25, 0.25), 0.25, 0.0 );
  SharedMaterialPtr blackDiffuseMaterial =Material::CreateMaterial(materialParam4);
  ShapeParam frontWallParam( Vec3(0, -1e5-150, 50), 1e5 );
  shapes.push_back( Shape::CreateShape( blackDiffuseMaterial, frontWallParam ) );

  // Floor
  ShapeParam floorParam( Vec3(0, 0, -1e5), 1e5 );
  shapes.push_back( Shape::CreateShape( grayDiffuseMaterial, floorParam ) );

  // Ceil
  ShapeParam ceilParam( Vec3(0, 0, 1e5+100), 1e5 );
  shapes.push_back( Shape::CreateShape( grayDiffuseMaterial, ceilParam ) );

  //Red Ball
  MaterialParam materialParam6( SPECULAR, Vec3(0.8, 0.8, 0.8), 0.99, 1.66 );
  SharedMaterialPtr greenDiffuseMaterial =Material::CreateMaterial(materialParam6);
  ShapeParam ballParam( Vec3(5, 80, 14), 8 );
  shapes.push_back( Shape::CreateShape( greenDiffuseMaterial, ballParam ) );



  //Specular Miku
  MaterialParam materialParam8( DIFFUSE, Vec3(0.4, 0.4, 0.2), 0.4, 1.5 );
  SharedMaterialPtr specDiffuseMaterial =Material::CreateMaterial(materialParam8);
  SharedTriangleMeshPtr miku1 =TriangleMesh::CreateTriangleMeshFromObjFile( "../ObjFiles/dragon.obj", specDiffuseMaterial, shapes);

  //miku1->Rotate(Vec3(1.0, 0.0, 0.0), -M_PI/2.0);
  miku1->Scaling(30.0);
  miku1->Move(Vec3(40, 70, 35));
  this->TriangleMeshes.push_back(miku1);


//   //Diffuse Miku
//   MaterialParam materialParam9( DIFFUSE, Vec3(0.25, 0.55, 0.25), 0.55, 0.0 );
//   SharedMaterialPtr difDiffuseMaterial =Material::CreateMaterial(materialParam9);
//   SharedTriangleMeshPtr miku2 =TriangleMesh::CreateTriangleMeshFromObjFile( "../ObjFiles/miku.obj", difDiffuseMaterial, shapes);

//   miku2->Rotate(Vec3(1.0, 0.0, 0.0), -M_PI/2.0);
//   miku2->Scaling(13.0);
//   miku2->Move(Vec3(-30, 110, 45));
//   this->TriangleMeshes.push_back(miku2);


  

//   //Refract Miku
//   MaterialParam materialParam10( REFRACTION, Vec3(1.0, 1.0, 1.0), 0.99, 1.66 );
//   SharedMaterialPtr refDiffuseMaterial =Material::CreateMaterial(materialParam10);
//   SharedTriangleMeshPtr miku3 =TriangleMesh::CreateTriangleMeshFromObjFile( "../ObjFiles/miku.obj", refDiffuseMaterial, shapes);

//   miku3->Rotate(Vec3(1.0, 0.0, 0.0), -M_PI/2.0);
//   miku3->Scaling(13.0);
//   miku3->Move(Vec3(-50, 80, 35));
//   this->TriangleMeshes.push_back(miku3);



  
  //Refract Bunny
  MaterialParam materialParamBun( REFRACTION, Vec3(0.7, 0.7, 0.9), 0.9, 1.66 );
  SharedMaterialPtr refDiffuseMaterial =Material::CreateMaterial(materialParamBun);
  SharedTriangleMeshPtr bunny =TriangleMesh::CreateTriangleMeshFromObjFile( "../ObjFiles/buddha.obj", refDiffuseMaterial, shapes);

  //bunny->Rotate(Vec3(1.0, 0.0, 0.0), -M_PI/2.0);
  bunny->Scaling(50.5);
  bunny->Move(Vec3(-20, 90, 25));
  this->TriangleMeshes.push_back(bunny);


  // //Diffuse Miku
  // MaterialParam materialParam9( DIFFUSE, Vec3(0.3, 0.6, 0.3), 0.6, 1.5 );
  // SharedMaterialPtr difDiffuseMaterial =Material::CreateMaterial(materialParam9);
  // SharedTriangleMeshPtr miku2 =TriangleMesh::CreateTriangleMeshFromObjFile( "../ObjFiles/teapot_without_normal.obj", difDiffuseMaterial, shapes);

  // miku2->Rotate(Vec3(1.0, 0.0, 0.0), -M_PI/2.0);
  // miku2->Scaling(13.0);
  // miku2->Move(Vec3(0, 110, 35));
  // this->TriangleMeshes.push_back(miku2);


  
  //BuildBVH
  SharedShapePtr bvh(new Bvh(shapes));
  this->ShapesBVH =bvh;
}



bool BunnyBox::GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info)
{
  return this->ShapesBVH->GetHitInformation(ray, tMax, tMin, info);
}



Ray BunnyBox::CalcNextRay(const Ray &ray, const Real rayParam, const SharedShapePtr &obj)
{
  assert(!"not implemented");
  return ray;
}
