#pragma once

#include "Core/Entity.h"

class Curve : public Entity {
public:
  Curve(Ogre::SceneNode *Root);

  virtual float GetLength() = 0;
  virtual Ogre::Vector3 Evaluate(const float &Distance) = 0;

  void ShowGizmos();

  int GetCurveId() const { return m_CurveID; }

  Ogre::Vector3 FindClosestPoint(const Ogre::Vector3 &Position,
                                 const float &Step = 1.0f);

private:
  int m_CurveID;
};
