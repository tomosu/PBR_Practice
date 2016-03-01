#pragma once

#include "Shape.h"
#include "HitInformation.h"

#include <string>
#include <cassert>
#include <memory>


//////////////////////
//
// Type def
//
//////////////////////
class Scene;
typedef std::shared_ptr<Scene> SharedScenePtr;

enum SceneType{
  CORNELL_BOX=0,
  BUNNY_BOX=1,
  BVH_TRIANGLE_SCENE=2
};



//////////////////////
//
// Scene
//
//////////////////////
class Scene
{
public:
  // factory
  static SharedScenePtr CreateScene(const SceneType sceneType, std::string filepath);
  virtual bool GetHitInformation(const Ray &ray, const Real tMax, const Real tMin, HitInformation* info){};
};
