#include "Scene.h"
#include "CornellBox.h"
#include "BunnyBox.h"
#include <cassert>

SharedScenePtr Scene::CreateScene(const SceneType sceneType, std::string filepath){

  switch(sceneType){
  case CORNELL_BOX:{
    SharedScenePtr ret(new CornellBox());
    return ret;
    break;
  }

  case BUNNY_BOX:{
    SharedScenePtr ret(new BunnyBox());
    return ret;
    break;
  }

  case BVH_TRIANGLE_SCENE:{
    assert(!"not implemented\n");
    //SharedScenePtr ret(new BVH_TriangleScene(filepath) );
    //return ret;
    break;
  }

    

  default:{
      assert(!"invalid scene type\n");
  }
  }// end switch
}
