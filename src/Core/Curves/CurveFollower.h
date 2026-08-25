#pragma once

#include "Core/Entity.h"
#include "Curve.h"

class CurveFollower : public Entity {
public:
  CurveFollower(Ogre::SceneNode *Root);

  virtual void Tick(const float &DeltaTime) override;

  bool HasCurve() { return m_Curve != nullptr; }
  void Follow(Curve *Curve, const float &Speed, const float &StartDistance = 0);

private:
  Curve *m_Curve;
  float m_Speed;
  float m_Distance;
};
