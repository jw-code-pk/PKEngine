#pragma once

#include <Ogre.h>

class TriggerProbe;

class Trigger {
public:
  virtual Ogre::Vector3 GetOrigin() = 0;

  virtual bool CheckOverlap(TriggerProbe *Probe) = 0;

  virtual void OnEnter(TriggerProbe *Probe) {};
  virtual void OnExit(TriggerProbe *Probe) {};
};
