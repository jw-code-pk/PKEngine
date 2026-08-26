#pragma once

#include "Core/Entity.h"
#include "Curve.h"

class CurveFollower : public Entity {
public:
  CurveFollower(Ogre::SceneNode *Root);

  virtual void Tick(const float &DeltaTime) override;

  inline bool IsCoyote() {
    return !HasCurve() || m_Distance < 0 || m_Distance > m_Curve->GetLength();
  }

  inline int CurrentCurveId() {
    return HasCurve() ? m_Curve->GetCurveId() : -1;
  }

  bool HasCurve() { return m_Curve != nullptr; }
  void Follow(Curve *Curve, const float &Speed, const float &StartDistance = 0);
  void SetSpeed(const float &Speed) { m_Speed = Speed; }

private:
  Curve *m_Curve;
  float m_Speed;
  float m_Distance;
};
