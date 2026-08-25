#pragma once

#include "Curve.h"

class CurveGroup {
public:
  void Register(Curve *Curve);
  void Unregister(Curve *Curve);

  bool TryGetClosest(const Ogre::Vector3 &Position, Curve *&Curve,
                     const int IgnoreId = -1) const;

private:
  std::unordered_map<int, Curve *> m_Curves;
};
