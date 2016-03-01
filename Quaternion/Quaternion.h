#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include "Vec3.h"
#include "RealTypeDef.h"


class Quaternion
{

 public:

  Quaternion(){};

  Quaternion(const Vec3 &axis, const Real radian)
    {
      const Real EPSILON_05 =0.000001;
      if(axis.Norm2() <EPSILON_05){
	std::cerr << "Invalid axis norm." << std::endl;
	exit(0);
      }
      Vec3 naxis =axis.Normalize();
    
      Q.clear();
      Q.push_back( cos( radian *0.5) );
      Real tmp =sin( radian *0.5);
      Q.push_back(naxis.x *tmp);
      Q.push_back(naxis.y *tmp);
      Q.push_back(naxis.z *tmp);
    };


  Quaternion(const Vec3 &point)
    {
      Q.clear();
      Q.push_back(0.0);
      Q.push_back(point.x);
      Q.push_back(point.y);
      Q.push_back(point.z);
    }


  Quaternion(const Real t, const Real x, const Real y, const Real z)
    {
      Q.clear();
      Q.push_back(t );
      Q.push_back(x );
      Q.push_back(y );
      Q.push_back(z );
    };


  Quaternion operator*(const Quaternion &obj) const
    {
      Real t =Q[0] *obj.Q[0]
	-Q[1] *obj.Q[1]
	-Q[2] *obj.Q[2]
	-Q[3] *obj.Q[3];

      Real x =Q[0] *obj.Q[1]
	+Q[1] *obj.Q[0]
	+Q[2] *obj.Q[3]
	-Q[3] *obj.Q[2];

      Real y =Q[0] *obj.Q[2]
	+Q[2] *obj.Q[0]
	+Q[3] *obj.Q[1]
	-Q[1] *obj.Q[3];

      Real z =Q[0] *obj.Q[3]
	+Q[3] *obj.Q[0]
	+Q[1] *obj.Q[2]
	-Q[2] *obj.Q[1];
      
      Quaternion ret(t,x,y,z);
      return ret;
    }


  Quaternion MakeConjugate() const
  {
    Quaternion ret( Q[0], -Q[1], -Q[2], -Q[3]);
    return ret;
  }

  void Print() const
  {
    std::cout << "(" << Q[0] << "," << Q[1] << "," << Q[2] << "," << Q[3] << ")" << std::endl;
  }

  std::vector<Real> Q;

};



class Rotor
{
 public:

  Rotor(const Vec3 &axis, Real radian)
    {
      Quaternion q1(axis, radian);
      Q1 =q1;
      Q2 =q1.MakeConjugate();
    }


  Vec3 Rotate(const Vec3 &in) const
  {
    Quaternion p1(in);
    Quaternion p2 =Q2 *p1 *Q1;
    return Quaternion2PointVector(p2);
  }
  

  Vec3 Quaternion2PointVector(const Quaternion &in) const
  {
    Vec3 ret;
    ret.x =in.Q[1];
    ret.y =in.Q[2];
    ret.z =in.Q[3];
    return ret;
  }


 private:
  Quaternion Q1;
  Quaternion Q2;

};
