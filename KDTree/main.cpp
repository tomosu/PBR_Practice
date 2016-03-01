#include <random>
#include <iostream>
#include <cmath>

#include "KDTree.h"

struct Point {
  Vec3 Position;
  Point(const Real x, const Real y, const Real z){Position.x =x; Position.y =y; Position.z =z;};
  void Print()const{ std::cout << Position.x << "," << Position.y << "," << Position.z << std::endl; }
};


inline Real rand01() { return (Real)rand()/(Real)RAND_MAX;}


int main()
{
  typedef KDTree<Point> SpaceTree;
  SpaceTree tree;
  for(int i=0; i<200; i++){
    tree.AddElement( Point(rand01(), rand01(), rand01()) );
  }
  tree.BuildTree();
  tree.PrintTree();


  for(int i=0; i<8; i++){
    Point center( rand01(), rand01(), rand01());

    center.Print();
    int searchNum =4;
    Real radius =5.0;
    Query query(radius, searchNum, center.Position);
    SpaceTree::GatheredElementsQueue out;
    tree.GatherElements(query, &out);

    std::cout << out.size() << std::endl;
    while(!out.empty()){
      out.top().ElementPtr->Print();
      std::cout << " dist" << out.top().Distance2 << std::endl;
      out.pop();
    }
    std::cout << std::endl;
  }
  return 0;
}
