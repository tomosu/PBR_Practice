#pragma once
#include <cmath>
#include <iostream>
#include <algorithm>
#include "RealTypeDef.h"
#include "Vec3.h"


struct SphericalCoordinateAngle_t{
  SphericalCoordinateAngle_t(){}
  SphericalCoordinateAngle_t(Real inclination, Real azimuth) : Inclination(inclination), Azimuth(azimuth) {}
  Real Inclination;
  Real Azimuth;
};


inline Real Crop(const Real min, const Real max, const Real val)
{
  return std::min(max, std::max(min, val));
}



inline SphericalCoordinateAngle_t ChangeAngle(const SphericalCoordinateAngle_t &origin,
					      const SphericalCoordinateAngle_t &displace)
{
  const Real EPSILON_05=0.000001;
  
  Real theta0 =origin.Inclination;
  Real psi0 =origin.Azimuth;
  Real thetaD =displace.Inclination;
  Real psiD =displace.Azimuth;
  Real cos_theta0 = cos(theta0);
  Real sin_theta0 = sin(theta0);
  Real cos_psi0 = cos(psi0);
  Real sin_psi0 = sin(psi0);
  Real cos_omega = cos(thetaD);
  Real sin_omega = sin(thetaD);
  Real cos_phi = cos(psiD);
  Real sin_phi = sin(psiD);
  
  //calculate polar1, azimuth
  Real cos_theta1 = cos_theta0 *cos_omega +sin_theta0 *sin_omega *cos_phi;
  Real sin_theta1 = sin( acos( Crop(-1.0, 1.0, cos_theta1) ));
  
  SphericalCoordinateAngle_t ret;
  
  Real sin_psi1_0 = sin(psiD) * sin_omega / sin_theta1;
  Real cos_psi1_0 = (cos_omega -cos_theta0 *cos_theta1) / (sin_theta0 *sin_theta1);

  if(fabs(sin_theta1) > EPSILON_05 && fabs(sin_theta0) > EPSILON_05){
    Real sin_psi1 = sin_psi0 * cos_psi1_0 + cos_psi0 * sin_psi1_0;
    Real cos_psi1 = cos_psi0 * cos_psi1_0 - sin_psi0 * sin_psi1_0;
    
    ret.Inclination =acos( Crop(-1.0, 1.0, cos_theta1) );
    ret.Azimuth =atan2(sin_psi1, cos_psi1);

  }else{
    ret.Inclination =acos( Crop(-1.0, 1.0, cos_theta1) );
    ret.Azimuth =psiD;

  }
  return ret;
}




inline Vec3 ConvertAngleToDirectionVector(const SphericalCoordinateAngle_t &in)
{
  Real theta=in.Inclination;
  Real psi=in.Azimuth;

  Vec3 vd;
  Real sin_theta =sin(theta);
  vd.x = sin_theta * cos(psi);
  vd.y = sin_theta * sin(psi);
  vd.z = cos(theta);

  return vd;
}



inline SphericalCoordinateAngle_t ConvertDirectionVectorToAngle(const Vec3 & vd)
{
  Real cos_theta =vd.z /sqrt(vd.x*vd.x + vd.y*vd.y +vd.z*vd.z);
  Real theta_tmp =acos(Crop(-1.0, 1.0, cos_theta) );
  Real psi_tmp =atan2(vd.y, vd.x);

  SphericalCoordinateAngle_t ret;
  ret.Inclination = theta_tmp;
  ret.Azimuth = psi_tmp;
  
  return ret;
}



inline Vec3 ChangeDirectionBySphericalCoordinateAngle(const Vec3 &origin,
						      const SphericalCoordinateAngle_t &displace)
{
  SphericalCoordinateAngle_t nd =ChangeAngle( ConvertDirectionVectorToAngle(origin), displace);
  return ConvertAngleToDirectionVector(nd);
}
