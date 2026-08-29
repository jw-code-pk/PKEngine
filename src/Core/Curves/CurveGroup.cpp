#include "CurveGroup.h"

#include <cassert>
#include <limits>

const float STEP_SIZE = 10.0f;
const float STEP_SIZE_SQ = STEP_SIZE * STEP_SIZE;

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
  auto closestPoint =
      Position + Ogre::Vector3::UNIT_SCALE * std::numeric_limits<float>::max();
  auto closestDistance = 0.0f;

  for (const auto &kvp : m_Curves) {
    if (kvp.first == IgnoreId) {
      continue;
    }

    auto c = kvp.second;
    auto p = c->FindClosestPoint(Position, STEP_SIZE);

    if (Position.squaredDistance(p.Point) > STEP_SIZE_SQ) {
      continue;
    }

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

CurveGroup::QueryResult
CurveGroup::FindClosestBelow(const Ogre::Vector3 &Position,
                             const int IgnoreId) const {
  assert(m_Curves.size() > 0 && "There are no curves registered.");

  Curve *closestCurve = nullptr;
  auto closestPoint =
      Ogre::Vector3(1, -1, 1) * std::numeric_limits<float>::max();

  auto closestDistance = 0.0f;

  for (const auto &kvp : m_Curves) {
    if (kvp.first == IgnoreId) {
      continue;
    }

    auto c = kvp.second;
    auto p0 = c->Evaluate(0);

    // ignore curves above our position
    if (p0.y > Position.y) {
      continue;
    }

    auto p = c->FindClosestPointIgnoreY(Position, STEP_SIZE);
    auto projC = p.Point;
    projC.y = 0;

    auto projP = Position;
    projP.y = 0;

    // check that the point is not too far away
    if (projP.squaredDistance(projC) > STEP_SIZE_SQ) {
      continue;
    }

    const auto d1 = projP.squaredDistance(closestPoint);
    const auto d2 = projP.squaredDistance(projC);

    // we are closer on the y axis already
    bool bIsCloserOnY = Position.y - p.Point.y < Position.y - closestPoint.y;

    if (d2 < d1 && bIsCloserOnY) {
      closestCurve = c;
      closestPoint = p.Point;
      closestDistance = p.Distance;
    }
  }

  return CurveGroup::QueryResult{.Curve = closestCurve,
                                 .Distance = closestDistance};
}
