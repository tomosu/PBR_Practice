#pragma once

#include "RealTypeDef.h"
#include "Vec3.h"

#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <cmath>
#include <queue>


struct Query
{
  Real SearchRadius;
  Real SearchRadius2;
  Vec3 SearchCenter;
  int SearchNum;
  
  Query(Real searchRadius,
	int searchNum,
	Vec3 &searchCenter)
    : SearchRadius(searchRadius),
      SearchNum(searchNum),
      SearchCenter(searchCenter)
  {
    SearchRadius2 =SearchRadius *SearchRadius;
  }

};



template<class T>
class KDTree
{
public:
  // type definition
  struct KDTreeNode
  {
    typedef std::shared_ptr<KDTreeNode> SharedKDTreeNodePtr;
    T* ElementPtr;
    int Axis;
    SharedKDTreeNodePtr Left;
    SharedKDTreeNodePtr Right;
  };
  
  typedef typename KDTreeNode::SharedKDTreeNodePtr SharedKDTreeNodePtr;
  
  typedef typename std::vector<T>::iterator ElementIterator;

  struct TaggedElement
  {
    TaggedElement(){}
    TaggedElement(const T *elementPtr_in, const double distance2_in)
      : ElementPtr(elementPtr_in), Distance2(distance2_in) {};

    const T *ElementPtr;
    Real Distance2;

    bool operator<(const TaggedElement &b) const {
      return b.Distance2 > Distance2;
    }
  };

  // output queue type
  typedef std::priority_queue<TaggedElement> GatheredElementsQueue;
  
  // 1. Add all elements.
  void AddElement(const T& element){ Elements.push_back(element); };

  // 2. Build tree.
  void BuildTree(){ Root =BuildSubTree(Elements.begin(), Elements.end(), 0); };

  // 3. Gather elements with query.
  void GatherElements(Query& query, GatheredElementsQueue* out){ GatherSubTreeElements(this->Root, query, out); };
  
  // for debug
  void PrintTree(){ PrintSubTree(this->Root, 0); };



private:
  
  SharedKDTreeNodePtr BuildSubTree(const ElementIterator begin, const ElementIterator end, int depth)
  {
    enum Axis{
      X=0,
      Y=1,
      Z=2
    };
    if(end == begin){ return nullptr;}
    
    int axis =depth %3;
    switch (axis) {
    case X: std::sort(begin, end, [](T const &left, T const &right){return left.Position.x < right.Position.x; }); break;
    case Y: std::sort(begin, end, [](T const &left, T const &right){return left.Position.y < right.Position.y; }); break;
    case Z: std::sort(begin, end, [](T const &left, T const &right){return left.Position.z < right.Position.z; }); break;
    }
    int median =(end -begin)/2;
    
    SharedKDTreeNodePtr ret(new KDTreeNode);
    ret->Axis =axis;
    ret->ElementPtr =&(*(begin+median));
    ret->Left =this->BuildSubTree(begin, begin +median, depth +1);
    ret->Right =this->BuildSubTree(begin +median +1, end, depth +1);    
    return ret;
  }
  
  void GatherSubTreeElements(const SharedKDTreeNodePtr& node, Query& query, GatheredElementsQueue* out)
  {
    if(node == nullptr){ return; }
    
    Vec3 diff =query.SearchCenter -node->ElementPtr->Position;
    Real diffSide;
    switch (node->Axis) {
    case 0: diffSide = diff.x; break;
    case 1: diffSide = diff.y; break;
    case 2: diffSide = diff.z; break;
    }
    
    Real distance2 =diff.Norm2();
    if(distance2 < query.SearchRadius2){
      out->push(TaggedElement(node->ElementPtr, distance2));
      if (out->size() > query.SearchNum) {
	out->pop();
	query.SearchRadius2 = out->top().Distance2;
      }
    }
    
    if(diffSide >0.0){
      GatherSubTreeElements(node->Right, query, out);
      if (diffSide *diffSide < query.SearchRadius2) {
	GatherSubTreeElements(node->Left, query, out);
      }
    }else{
      GatherSubTreeElements(node->Left, query, out);
      if (diffSide *diffSide < query.SearchRadius2) {
	GatherSubTreeElements(node->Right, query, out);
      }
    }
  }
  
  void PrintSubTree(const SharedKDTreeNodePtr& node, int lineDepth)
  {
    for(int i=0; i<lineDepth; i++){std::cout << " ";}
    node->ElementPtr->Print();

    if(node->Left != nullptr)
      PrintSubTree(node->Left, lineDepth+2);

    if(node->Right != nullptr)
      PrintSubTree(node->Right, lineDepth+2);
  }
  
  SharedKDTreeNodePtr Root;
  std::vector<T> Elements;
};
