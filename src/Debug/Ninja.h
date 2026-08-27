#pragma once

#include "NinjaSM/NinjaSM.h"

#include "Core/Curves/CurveFollower.h"
#include <OgrePrerequisites.h>

class Ninja : public CurveFollower {
public:
  Ninja(Ogre::SceneNode *Root);
  ~Ninja();

  virtual Ogre::String GetTypeId() override { return "Ninja"; }
  virtual bool Init() override;

  virtual void BeginPlay() override;
  virtual void Tick(const float &DeltaTime) override;
  virtual void EndPlay() override;

  void ChangeAnim(const Ogre::String &Name);
  void FaceDirection(const Ogre::Vector3 &Direction);
  NinjaSM *GetStateMachine() { return m_StateMachine; }

private:
  float m_Rotation;
  Ogre::SceneNode *m_PawnNode;
  Ogre::AnimationState *m_ActiveAnim;
  Ogre::AnimationStateSet *m_AnimSet;

  NinjaSM *m_StateMachine;
};
