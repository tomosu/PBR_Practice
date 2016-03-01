#include "TriangleMesh.h"

int main()
{
  MaterialParam materialParam( DIFFUSE, Vec3(0.75, 0.25, 0.25), 0.75, 0.0 );
  SharedMaterialPtr material =Material::CreateMaterial(materialParam);

  std::vector<SharedShapePtr> shapes;
  SharedTriangleMeshPtr mesh =TriangleMesh::CreateTriangleMeshFromObjFile("bunny.obj", material, shapes);
  mesh->Scaling(2.0);
  mesh->Move(Vec3(3.0, 3.0, 3.0));
  
  return 0;
}
