#pragma once
#include "PhotonInteraction.h"
#include "KDTree.h"
#include "Scene.h"
#include "Light.h"

struct Photon
{
  Photon(const Vec3 &position, const Vec3 &direction, const Vec3 &color)
    :Position(position), Direction(direction), Color(color)
  {}
  Vec3 Position;
  Vec3 Direction;
  Vec3 Color;
};

class PhotonMap
{
public:
  PhotonMap(SharedLightPtr light, SharedScenePtr scene)
    : Light(light), Scene(scene)
  {}

  void ShootPhotons(const int photonNum);
  
  Vec3 GetRadiance(UniformRandomGenerator01 &random, const Ray &ray, const int depth, const Real searchRadius, const int searchNum);
  
private:
  std::vector<Photon> ShootPhotonsAsync(const int photonNum, const unsigned int seed);
  SharedLightPtr Light;
  SharedScenePtr Scene;
  
  typedef KDTree<Photon> PhotonMapTree;
  PhotonMapTree Photons;
};
