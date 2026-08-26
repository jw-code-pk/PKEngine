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

bool CurveGroup::TryGetClosest(const Ogre::Vector3 &Position, Curve *&Curve,
                               const int IgnoreId) const {
  assert(m_Curves.size() > 0 && "There are no curves registered.");

  auto tolerance = 10.0f;
  Curve = m_Curves.begin()->second;
  auto closestPoint = Curve->FindClosestPoint(Position, tolerance);

  for (const auto &kvp : m_Curves) {
    if (kvp.first == IgnoreId) {
      continue;
    }

    auto c = kvp.second;
    auto p = c->FindClosestPoint(Position, tolerance);

    const auto d1 = Position.squaredDistance(closestPoint);
    const auto d2 = Position.squaredDistance(p);

    if (d2 < d1) {
      Curve = c;
      closestPoint = p;
    }
  }

  return Curve != nullptr && Curve->GetCurveId() != IgnoreId;
}
