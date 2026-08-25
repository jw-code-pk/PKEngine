#pragma once

#include "Core/Entity.h"

class Curve : public Entity {
public:
  Curve(Ogre::SceneNode *Root) : Entity(Root) {}

  virtual float GetLength() = 0;
  virtual Ogre::Vector3 Evaluate(const float &Distance) = 0;

  void ShowGizmos();
};
