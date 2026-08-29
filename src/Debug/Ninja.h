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

  bool IsGrounded() { return m_IsGrounded; }
  void Launch(const float &Speed);

protected:
  void TickPhysics(const float &DeltaTime);
  void TickGroundMovement(const float &DeltaTime);
  void TickInAirMovement(const float &DeltaTime);

private:
  float m_Rotation;

  float m_GroundDistance;
  float m_VerticalSpeed;
  bool m_IsGrounded;

  Ogre::SceneNode *m_PawnNode;
  Ogre::SceneNode *m_ModelNode;

  Ogre::AnimationState *m_ActiveAnim;
  Ogre::AnimationStateSet *m_AnimSet;

  NinjaSM *m_StateMachine;
};
