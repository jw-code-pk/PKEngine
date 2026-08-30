#include "CurveGroup.h"

#include <cassert>
#include <limits>

const float STEP_SIZE = 10.0f;
const float STEP_SIZE_SQ = STEP_SIZE * STEP_SIZE;

CurveGroup::CurveGroup() {
  m_CurveTree = new Octree<Curve *>(Ogre::Vector3::ZERO,
                                    Ogre::Vector3::UNIT_SCALE * 35000);
}

CurveGroup::~CurveGroup() {
  if (m_CurveTree != nullptr) {
    m_CurveTree->ClearAll();
    delete m_CurveTree;
  }
}

void CurveGroup::Register(Curve *Curve) {
  assert(Curve && "Curve should not be null.");

  auto key = Curve->GetCurveId();
  m_Curves[key] = Curve;

  RefreshTree();
}

void CurveGroup::Unregister(Curve *Curve) {
  assert(Curve && "Curve should not be null.");

  auto key = Curve->GetCurveId();
  m_Curves.erase(key);

  RefreshTree();
}

CurveGroup::QueryResult CurveGroup::FindClosest(const Ogre::Vector3 &Position,
                                                const int &IgnoreId,
                                                const float &Range) const {
  assert(m_Curves.size() > 0 && "There are no curves registered.");

  Curve *closestCurve = nullptr;
  auto closestPoint =
      Position + Ogre::Vector3::UNIT_SCALE * std::numeric_limits<float>::max();
  auto closestDistance = 0.0f;

  std::set<Curve *> curveResults;
  m_CurveTree->FetchAll(Position, Ogre::Vector3(Range, Range, Range),
                        curveResults);

  for (const auto c : curveResults) {
    if (c->GetCurveId() == IgnoreId) {
      continue;
    }

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
CurveGroup::FindClosestBelow(const Ogre::Vector3 &Position, const int &IgnoreId,
                             const float &Range) const {
  assert(m_Curves.size() > 0 && "There are no curves registered.");

  Curve *closestCurve = nullptr;
  auto closestPoint =
      Ogre::Vector3(1, -1, 1) * std::numeric_limits<float>::max();

  auto closestDistance = 0.0f;

  std::set<Curve *> curveResults;
  m_CurveTree->FetchAll(Position, Ogre::Vector3(Range, Range, Range),
                        curveResults);

  for (const auto c : curveResults) {
    if (c->GetCurveId() == IgnoreId) {
      continue;
    }

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

void CurveGroup::RefreshTree() {
  if (m_CurveTree != nullptr) {
    m_CurveTree->ClearAll();
    delete m_CurveTree;
  }

  auto maxDistanceSq = 0;

  for (auto kvp : m_Curves) {
    auto curve = kvp.second;
    auto p0 = curve->Evaluate(0);
    auto p1 = curve->Evaluate(curve->GetLength());

    if (p0.squaredLength() > maxDistanceSq) {
      maxDistanceSq = p0.squaredLength();
    }

    if (p1.squaredLength() > maxDistanceSq) {
      maxDistanceSq = p1.squaredLength();
    }
  }

  auto dim =
      100 + Ogre::Math::Sqrt(maxDistanceSq); // adding 100 for some padding

  m_CurveTree =
      new Octree<Curve *>(Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_SCALE * dim);

  for (auto kvp : m_Curves) {
    auto curve = kvp.second;
    auto startPoint = curve->Evaluate(0);
    auto endPoint = curve->Evaluate(curve->GetLength());

    m_CurveTree->TryAdd(startPoint, curve);
    m_CurveTree->TryAdd(endPoint, curve);
  }
}
