#include "TriangleMesh.h"
#include "StringUtil.h"
#include "Quaternion.h"


// assume in represents "v/vt/vn" or "v/vn"
SharedShapePtr TriangleMesh::CreateTriangleFaceFromToken(SharedMaterialPtr &material,
							 const std::string &inA, const std::string &inB, const std::string &inC)
{
  std::string delim("/");
  
  std::vector<std::string> tokenA =my_split(inA, delim);
  int vidxA =my_stoi( tokenA.front() )-1; //dont use texture
  Vec3 &vA =this->Vertex[vidxA];
   
  std::vector<std::string> tokenB =my_split(inB, delim);
  int vidxB =my_stoi( tokenB.front() )-1; //dont use texture
  Vec3 &vB =this->Vertex[vidxB];

  std::vector<std::string> tokenC =my_split(inC, delim);
  int vidxC =my_stoi( tokenC.front() )-1; //dont use texture
  Vec3 &vC =this->Vertex[vidxC];
  
  if(this->UseNormal){
    int vnidxA =my_stoi( tokenA.back() )-1; //dont use texture
    Vec3 &vnA =this->NormalVectors[vnidxA];
    int vnidxB =my_stoi( tokenB.back() )-1; //dont use texture
    Vec3 &vnB =this->NormalVectors[vnidxB];
    int vnidxC =my_stoi( tokenC.back() )-1; //dont use texture
    Vec3 &vnC =this->NormalVectors[vnidxC];
    return Shape::CreateTriangleFace(material, vA, vB, vC, vnA, vnB, vnC);
  }

  if(!this->UseNormal){
    Vec3 faceNormal =((vC-vA).Cross(vB-vA)).Normalize();
    this->NormalVectors.push_back(faceNormal); // capacity reserved
    Vec3 &vnA =this->NormalVectors.back();
    Vec3 &vnB =this->NormalVectors.back();
    Vec3 &vnC =this->NormalVectors.back();
    return Shape::CreateTriangleFace(material, vA, vB, vC, vnA, vnB, vnC);
  }
}


int CountFaceNum(const std::string &filepath)
{
  std::ifstream ifs(filepath.c_str());
  if(ifs.fail()){ std::cout << "file not found:" << filepath << std::endl; }
  
  std::string buf;
  std::string wspace(" ");

  int faceNum =0;
  while( getline(ifs, buf) ){
    std::vector<std::string> token =my_split(buf, wspace);
    
    if(token[0]=="f"){ // face
      if(token.size() == 4 || token.size() == 5){
	faceNum++;
      }else{
	std::cout << "invalid face size" << std::endl;
      }
    }
  }
  std::cout << "face num:" << faceNum << std::endl;
  return faceNum;
}


SharedTriangleMeshPtr TriangleMesh::CreateTriangleMeshFromObjFile(const std::string &filepath,
								  SharedMaterialPtr &material,
								  std::vector<SharedShapePtr> &shapePool)
{  
  TriangleMesh* ret =new TriangleMesh();

  ///////////////////////////
  // reserve NormalVectors
  ///////////////////////////
  ret->ReserveNormalVectors(CountFaceNum(filepath));

  /////////////////
  // open file
  /////////////////
  std::ifstream ifs(filepath.c_str());
  if(ifs.fail()){ std::cout << "file not found:" << filepath << std::endl; }
  
  std::string buf;
  std::string wspace(" ");

  while( getline(ifs, buf) ){
    std::vector<std::string> token =my_split(buf, wspace);

    if(token[0] =="#"){}//comment

    if(token[0] =="o"){}//not implemented

    if(token[0] =="mtllib"){}//not implemented

    if(token[0] =="usemtl"){}//not implemented

    if(token[0] =="v"){ // vertex
      if(token.size() == 4 || token.size() == 5){
	ret->Vertex.push_back( Vec3(my_stor(token[1]), my_stor(token[2]), my_stor(token[3])) );
      }else{
	std::cout << "invalid vertex size" << std::endl;
      }
    }

    if(token[0]=="vn"){ // normal vector
      if(token.size() == 4 || token.size() == 5){
	ret->UseNormal =true;
	ret->NormalVectors.push_back( Vec3(my_stor(token[1]), my_stor(token[2]), my_stor(token[3])) );
      }else{
	std::cout << "invalid normal vector size" << std::endl;
      }
    }

    if(token[0]=="vt"){ // texture
      if(token.size() == 4 || token.size() == 5){
	ret->UseTexture =true;
	// not implemented
	//ret->Texture.push_back( Vec3(my_stor(token[1]), my_stor(token[2]), my_stor(token[3])) );
      }else{
	//std::cout << "invalid texture size" << std::endl;
      }
    }

    if(token[0]=="f"){ // face
      if(token.size() == 4 || token.size() == 5){
	SharedShapePtr face =ret->CreateTriangleFaceFromToken(material, token[1], token[2], token[3]);
	shapePool.push_back(face);
      }else{
	std::cout << "invalid face size" << std::endl;
      }
    }
    
  }//read all data
  
  return SharedTriangleMeshPtr(ret);
}



void TriangleMesh::Scaling(const Real scale)
{
  for(int i=0; i<Vertex.size(); i++){
    Vertex[i]=Vertex[i]*scale;
  }
}



void TriangleMesh::Move(const Vec3 &move)
{
  for(int i=0; i<Vertex.size(); i++){
    Vertex[i]=Vertex[i]+move;
  }
}



void TriangleMesh::Rotate(const Vec3 &axis, const Real radian)
{
  Rotor rotor(axis, radian);
  for(int i=0; i<Vertex.size(); i++){
    Vertex[i]=rotor.Rotate(Vertex[i]);
  }

  for(int i=0; i<NormalVectors.size(); i++){
    NormalVectors[i]=rotor.Rotate(NormalVectors[i]);
  }
}
