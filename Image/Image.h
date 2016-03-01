#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

#include "Vec3.h"
#include "RealTypeDef.h"

class Image;
typedef std::shared_ptr<Image> SharedImagePtr;


class Image
{
public:

  Image(const int sizeU, const int sizeV)
  {
    this->SizeU =sizeU;
    this->SizeV =sizeV;
    this->Pixels.resize(SizeU *SizeV);
  }
  
  static SharedImagePtr CreateImage(const int sizeU, const int sizeV)
  {
    SharedImagePtr ret(new Image(sizeU, sizeV));
    return ret;
  }

  void SetVal(const int u, const int v, const Vec3 &val)
  {
    this->Pixels[u +this->SizeU *v] =val;
  }

  void ExportPPM(const std::string &filepath)
  {
    FILE *f = fopen(filepath.c_str(), "wb");

    fprintf(f, "P3\n%d %d\n%d\n", SizeU, SizeV, 255);
    for (int i = 0; i < SizeU * SizeV; i++){
      fprintf(f,"%d %d %d ", Map2Int(Pixels[i].x), Map2Int(Pixels[i].y), Map2Int(Pixels[i].z) );
    }
    fclose(f);
  }

  void Gamma(const Real gamma){};

  void Binning(const int ubin, const int vbin)
  {

    std::vector<Vec3> tmpPixels;
    

    for(int v=0; v< SizeV; v+=vbin){
      for(int u=0; u< SizeU; u+=ubin){
	Vec3 pixel(0.0, 0.0, 0.0);
	int binnum =0;
	
	for(int dv=0; dv < vbin; dv++){
	  for(int du=0; du < ubin; du++){
	    if( u +du < SizeU && v +dv < SizeV  ){
	      pixel = pixel +Pixels[u +du +SizeU*(v +dv)];
	      binnum++;
	    }
	  }
	}
	tmpPixels.push_back(pixel/(Real)binnum);
      }
    }

    this->SizeU = this->SizeU/ubin;
    this->SizeV = this->SizeV/vbin;
    
    for(int i=0; i<SizeU*SizeV; i++){
      Pixels[i]=tmpPixels[i];
    }
  }

private:

  Real Clamp(const Real in){ return (Real)std::min(1.0, std::max(0.0, (double)in)); }
  int Map2Int(const Real in){ return int(pow(Clamp(in), 1/2.2) * 255 + 0.5);}

  int SizeU;
  int SizeV;
  std::vector<Vec3> Pixels;
};
