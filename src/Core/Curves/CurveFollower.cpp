#include "CurveFollower.h"
#include <cassert>

CurveFollower::CurveFollower(Ogre::SceneNode *Root) : Entity(Root) {
  m_CanTick = true;
}

void CurveFollower::Tick(const float &DeltaTime) {
  if (HasCurve()) {
    m_Distance += m_Speed * DeltaTime;
    auto updatedPos = m_Curve->Evaluate(m_Distance);
    GetRoot()->setPosition(updatedPos);
  }
}

void CurveFollower::Follow(Curve *Curve, const float &Speed,
                           const float &StartDistance) {
  assert(Curve && "A valid curve needs to be supplied.");

  m_Curve = Curve;
  m_Speed = Speed;
  m_Distance = StartDistance;

  auto updatedPos = m_Curve->Evaluate(m_Distance);
  GetRoot()->setPosition(updatedPos);
}
