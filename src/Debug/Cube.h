#pragma once

#include "Core/Entity.h"

class Cube : public Entity {
public:
  Cube(Ogre::SceneNode *Root);

  virtual Ogre::String GetTypeId() override { return "Cube"; }

  virtual bool Init() override;
};
