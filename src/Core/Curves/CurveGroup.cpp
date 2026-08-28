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

CurveGroup::QueryResult CurveGroup::FindClosest(const Ogre::Vector3 &Position,
                                                const int IgnoreId) const {

  assert(m_Curves.size() > 0 && "There are no curves registered.");

  Curve *closestCurve = nullptr;
  auto stepSize = 10.0f;
  auto closestPoint = Position + Ogre::Vector3::UNIT_SCALE * 100;
  auto closestDistance = 0.0f;

  for (const auto &kvp : m_Curves) {
    if (kvp.first == IgnoreId) {
      continue;
    }

    auto c = kvp.second;
    auto p = c->FindClosestPoint(Position, stepSize);

    const auto d1 = Position.squaredDistance(closestPoint);
    const auto d2 = Position.squaredDistance(p.Point);

    if (d2 < d1) {
      closestCurve = c;
      closestPoint = p.Point;
      closestDistance = p.Distance;
    }
  }

  return CurveGroup::QueryResult{.Curve = closestCurve,
                                 .Distance = closestDistance};
}
