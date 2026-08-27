#pragma once

#include "Core/Curves/CurveFollower.h"
#include <OgrePrerequisites.h>

class Ninja : public CurveFollower {
public:
  Ninja(Ogre::SceneNode *Root);

  virtual Ogre::String GetTypeId() override { return "Ninja"; }
  virtual bool Init() override;
  virtual void Tick(const float &DeltaTime) override;

protected:
  void ChangeAnim(const Ogre::String &Name);

private:
  float m_Rotation;
  Ogre::SceneNode *m_PawnNode;
  Ogre::AnimationState *m_ActiveAnim;
  Ogre::AnimationStateSet *m_AnimSet;
};
