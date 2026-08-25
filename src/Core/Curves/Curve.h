#pragma once

#include "Core/Entity.h"

class Curve : public Entity {
public:
  Curve(Ogre::SceneNode *Root);

  virtual float GetLength() = 0;
  virtual Ogre::Vector3 Evaluate(const float &Distance) = 0;

  void ShowGizmos();

  int GetCurveId() const { return m_CurveID; }

private:
  int m_CurveID;
};
