#pragma once

#include "Core/Entity.h"
#include "Curve.h"
#include "CurveGroup.h"

class CurveFollower : public Entity {
public:
  CurveFollower(Ogre::SceneNode *Root);

  virtual void Tick(const float &DeltaTime) override;

  inline bool IsCoyote() {
    return !HasCurve() || m_Distance < 0 || m_Distance > m_Curve->GetLength();
  }

  inline bool CalculateCoyoteDistance() {
    if (HasCurve()) {
      return m_Distance < 0 ? -1 * m_Distance
                            : m_Distance - m_Curve->GetLength();
    } else {
      return 0;
    }
  }

  inline int CurrentCurveId() {
    return HasCurve() ? m_Curve->GetCurveId() : -1;
  }

  bool HasCurve() { return m_Curve != nullptr; }
  void Follow(Curve *Curve, const float &StartDistance = 0);
  void FollowClosest(const Ogre::Vector3 &Position, CurveGroup *Group,
                     const bool &bIgnoreCurrent = false);
  void FollowClosestBelow(const Ogre::Vector3 &Position, CurveGroup *Group,
                          const bool &bIgnoreCurrent = false);

  void SetSpeed(const float &Speed) { m_Speed = Speed; }

  Ogre::Quaternion CalculateOrientation();

private:
  Curve *m_Curve;
  float m_Speed;
  float m_Distance;
  Ogre::Vector3 m_Forward;
};
