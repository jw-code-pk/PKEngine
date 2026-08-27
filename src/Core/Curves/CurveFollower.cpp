#include "CurveFollower.h"
#include <cassert>

CurveFollower::CurveFollower(Ogre::SceneNode *Root) : Entity(Root) {
  m_CanTick = true;
  m_Forward = Ogre::Vector3::UNIT_X;
}

void CurveFollower::Tick(const float &DeltaTime) {
  if (!HasCurve()) {
    return;
  }

  m_Distance += m_Speed * DeltaTime;

  auto currentPos = GetRoot()->getPosition();
  auto updatedPos = currentPos;

  if (!IsCoyote()) {
    updatedPos = m_Curve->Evaluate(m_Distance);

    auto offset = currentPos - updatedPos;
    if (offset.squaredLength() > 0) {
      m_Forward = offset.normalisedCopy();
    }
  }

  GetRoot()->setPosition(updatedPos);
}

void CurveFollower::Follow(Curve *Curve, const float &StartDistance) {
  assert(Curve && "A valid curve needs to be supplied.");

  m_Curve = Curve;
  m_Distance = StartDistance;

  assert(!IsCoyote() && "Can't follow a curve with out of scope distance.");

  auto updatedPos = m_Curve->Evaluate(m_Distance);
  GetRoot()->setPosition(updatedPos);
}

void CurveFollower::FollowClosest(const Ogre::Vector3 &Position,
                                  CurveGroup *Group) {
  auto queryResult = Group->FindClosest(Position, CurrentCurveId());
  if (queryResult.IsValid()) {
    Follow(queryResult.Curve, queryResult.Distance);
  }
}

Ogre::Quaternion CurveFollower::CalculateOrientation() {
  return Ogre::Vector3::UNIT_Z.getRotationTo(m_Forward);
}
