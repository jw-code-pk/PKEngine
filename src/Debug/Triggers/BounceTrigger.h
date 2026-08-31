#pragma once

#include "Core/Entity.h"
#include "Core/Triggers/Trigger.h"

class BounceTrigger : public Entity, public Trigger {
public:
  virtual Ogre::String GetTypeId() override { return "Bounce"; }

  BounceTrigger(Ogre::SceneNode *Root);

  virtual Ogre::AxisAlignedBox GetAAB() override;

  virtual bool Init() override;

  virtual void BeginPlay() override;
  virtual void EndPlay() override;

  virtual bool CheckOverlap(TriggerProbe *Probe) override;
  virtual void OnEnter(TriggerProbe *Probe) override;
  virtual void OnExit(TriggerProbe *Probe) override;
};
