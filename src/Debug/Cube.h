#pragma once

#include "Core/Entity.h"

class Cube : public Entity {
public:
  Cube(Ogre::SceneNode *Root);

  virtual bool Init() override;
};
