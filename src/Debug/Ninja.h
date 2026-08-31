#pragma once
#include "NinjaSM/NinjaSM.h"

#include "Core/Triggers/TriggerProbe.h"
#include "Core/Triggers/TriggerSweeper.h"

#include "Core/Curves/CurveFollower.h"
#include <OgrePrerequisites.h>

class Ninja : public CurveFollower, public TriggerProbe {
public:
  virtual Ogre::String GetTypeId() override { return "Ninja"; }

  Ninja(Ogre::SceneNode *Root);
  ~Ninja();

  // Entity Lifecycle

  virtual bool Init() override;
  virtual void BeginPlay() override;
  virtual void Tick(const float &DeltaTime) override;
  virtual void EndPlay() override;

  // Trigger Probe

  virtual Ogre::AxisAlignedBox GetAAB() override;
  virtual bool OnEnter(Trigger *Tripped) override;
  virtual bool OnExit(Trigger *Tripped) override;

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

  TriggerSweeper *m_TriggerSweeper;
  NinjaSM *m_StateMachine;
};
