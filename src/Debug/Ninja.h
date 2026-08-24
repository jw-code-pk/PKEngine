#pragma once

#include "Core/Entity.h"

class Ninja : public Entity {
public:
  Ninja(Ogre::SceneNode *Root);

  virtual bool Init() override;

  virtual void Tick(const float &DeltaTime) override;

private:
  float m_Rotation;
  Ogre::SceneNode *m_PawnNode;
};
