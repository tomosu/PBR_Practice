#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "Shape.h"
#include "Vec3.h"

class TriangleMesh;
typedef std::shared_ptr<TriangleMesh> SharedTriangleMeshPtr;

class TriangleMesh
{
public:
  friend class TriangleFace;

  TriangleMesh() : UseNormal(false), UseTexture(false) {}

  static SharedTriangleMeshPtr CreateTriangleMeshFromObjFile(const std::string &filepath,
							     SharedMaterialPtr &material,
							     std::vector<SharedShapePtr> &shapePool);
  void Scaling(const Real scale);
  void Move(const Vec3 &move);
  void Rotate(const Vec3 &axis, const Real radian);
  
private:
  void ReserveNormalVectors(const int num){this->NormalVectors.reserve(num);}
  SharedShapePtr CreateTriangleFaceFromToken(SharedMaterialPtr &material, const std::string &inA, const std::string &inB, const std::string &inC);
  std::vector<Vec3> Vertex;
  std::vector<Vec3> NormalVectors;
  bool UseNormal;
  bool UseTexture;
};
