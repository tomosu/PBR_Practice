#pragma once

#include <memory>
#include "Image.h"


class Camera;
typedef std::shared_ptr<Camera> SharedCameraPtr;

struct CameraParam
{
  CameraParam(Vec3 eye, Vec3 normalVector, Vec3 upVector, Vec3 screenCenter,
	      Real pitchU, Real pitchV, int sizeU, int sizeV)
    :Eye(eye), NormalVector(normalVector), UpVector(upVector), ScreenCenter(screenCenter),
     PitchU(pitchU),PitchV(pitchV), SizeU(sizeU), SizeV(sizeV) {}

  Vec3 Eye;
  Vec3 NormalVector;
  Vec3 UpVector;
  Vec3 ScreenCenter;
  Real PitchU;
  Real PitchV;
  int SizeU;
  int SizeV;
};


class Camera
{

public:

  Camera(const CameraParam &param)
    :Eye(param.Eye), ScreenCenter(param.ScreenCenter),
     PitchU(param.PitchU), PitchV(param.PitchV),
     SizeU(param.SizeU), SizeV(param.SizeV)
  {
    // set screen
    this->Screen =Image::CreateImage(SizeU, SizeV);

    //set Uvec, Vvec
    this->NormalVector =param.NormalVector.Normalize();
    this->VVector =param.UpVector.Normalize();
    this->UVector =this->NormalVector.Cross( this->VVector);
  }

  static SharedCameraPtr CreateCamera(const CameraParam &param)
  {
    SharedCameraPtr ret(new Camera(param));
    return ret;
  };
  
  Ray GeneratePixelCenterRay(const int u, const int v)
  {
    Vec3 detOrigin =this->ScreenCenter -(this->UVector *this->PitchU *SizeU)/2.0
      +(this->VVector *this->PitchV *SizeV)/2.0;
    Vec3 destination =detOrigin +(this->UVector *this->PitchU *u) -(this->VVector *this->PitchV *v);
    Vec3 dir = (destination -this->Eye);

    return Ray(this->Eye, dir);
  }

  void SetImageValue(const int u, const int v, const Vec3 &val)
  {
    this->Screen->SetVal(u, v, val);
  }

  void ExportPPM(const std::string &filepath)
  {
    this->Screen->ExportPPM(filepath);
  }

  void Binning(const int ubin, const int vbin)
  {
    this->Screen->Binning(ubin, vbin);
  }
    
  int GetSizeU() const { return SizeU;}
  int GetSizeV() const { return SizeV;}

private:

  Vec3 Eye;
  Vec3 NormalVector;
  Vec3 UVector;
  Vec3 VVector;
  Vec3 ScreenCenter;
  Real PitchU;
  Real PitchV;
  int SizeU;
  int SizeV;

  SharedImagePtr Screen;
};
