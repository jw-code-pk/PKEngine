#pragma once

#include "Core/Curves/CurveFollower.h"
#include <OgrePrerequisites.h>

class Ninja : public CurveFollower {
public:
  Ninja(Ogre::SceneNode *Root);

  virtual bool Init() override;

  virtual void Tick(const float &DeltaTime) override;

  virtual Ogre::String GetTypeId() override { return "Ninja"; }

private:
  float m_Rotation;
  Ogre::SceneNode *m_PawnNode;
};
