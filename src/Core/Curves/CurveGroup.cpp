#include "CurveGroup.h"

#include <cassert>

void CurveGroup::Register(Curve *Curve) {
  assert(Curve && "Curve should not be null.");

  auto key = Curve->GetCurveId();
  m_Curves[key] = Curve;
}

void CurveGroup::Unregister(Curve *Curve) {
  assert(Curve && "Curve should not be null.");

  auto key = Curve->GetCurveId();
  m_Curves.erase(key);
}

bool CurveGroup::TryGetClosest(const Ogre::Vector3 &Position,
                               Curve *&Curve) const {
  for (const auto &kvp : m_Curves) {
    auto c = kvp.second;
    Curve = c;
    return true;
  }

  return false;
}
