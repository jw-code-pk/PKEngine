#pragma once

#include "Core/Octree.h"
#include "Curve.h"

class CurveGroup {
public:
  struct QueryResult {
    Curve *Curve;
    float Distance;

    bool IsValid() { return Curve != nullptr; }
  };

  void Register(Curve *Curve);
  void Unregister(Curve *Curve);

  QueryResult FindClosest(const Ogre::Vector3 &Position,
                          const int &IgnoreId = Curve::NoneID,
                          const float &Range = 1000) const;

  QueryResult FindClosestBelow(const Ogre::Vector3 &Position,
                               const int &IgnoreId = Curve::NoneID,
                               const float &Range = 1000) const;

  void RefreshTree();

  Octree<Curve *> *GetTree() { return m_CurveTree; }

private:
  std::unordered_map<int, Curve *> m_Curves;
  Octree<Curve *> *m_CurveTree;
};
