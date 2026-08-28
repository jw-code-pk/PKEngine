#pragma once

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
                          const int IgnoreId = Curve::NoneID) const;

  QueryResult FindClosestBelow(const Ogre::Vector3 &Position,
                               const int IgnoreId = Curve::NoneID) const;

private:
  std::unordered_map<int, Curve *> m_Curves;
};
