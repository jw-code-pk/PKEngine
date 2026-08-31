#pragma once

#include <Ogre.h>

class TriggerProbe;

class Trigger {
public:
  virtual Ogre::AxisAlignedBox GetAAB() = 0;

  virtual ~Trigger() = default;

  virtual bool CheckOverlap(TriggerProbe *Probe) = 0;

  virtual void OnEnter(TriggerProbe *Probe) {};
  virtual void OnExit(TriggerProbe *Probe) {};
};
