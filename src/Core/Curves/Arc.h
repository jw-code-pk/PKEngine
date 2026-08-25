#pragma once

#include "Curve.h"

class Arc : public Curve {
public:
  Arc(Ogre::SceneNode *Root);

  virtual Ogre::String GetTypeId() override { return "Arc"; }
  virtual bool Init() override;
  virtual float GetLength() override;
  virtual Ogre::Vector3 Evaluate(const float &Distance) override;

  void Setup(const float &Radius, const Ogre::Degree &Angle);
  void ShowGizmos();

private:
  float m_Radius;
  Ogre::Degree m_Angle;
};
