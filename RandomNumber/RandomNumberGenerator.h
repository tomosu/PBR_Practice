#pragma once
#include <random>
#include <cmath>
#include <functional>
#include "Vec3.h"
#include "RealTypeDef.h"

//#define M_PI 3.1415926535897932384626433832795

class UniformRandomGenerator01
{    
public:
  UniformRandomGenerator01(unsigned int seed_in)
  {
    MT.seed(seed_in);
    std::uniform_real_distribution<Real> dist(0.0, 1.0);
    Rand =std::bind(dist, MT);
  };
   
  Real GenRand(){return Rand();}

private:
  std::mt19937 MT;
  std::function<Real()> Rand;
};


inline Vec3 RandomOnUnitSphere(UniformRandomGenerator01 &random)
{
  Vec3 ret;
  Real t =random.GenRand() *2.0 *M_PI;
  Real f =random.GenRand() *2.0 *M_PI;
  ret.x =sin(t) *cos(f);
  ret.y =sin(t) *sin(f);
  ret.z =cos(t);
  return ret;
}
