#pragma once

#include <Ogre.h>

class Trigger;

class TriggerProbe {
public:
  virtual Ogre::AxisAlignedBox GetAAB() = 0;

  virtual bool OnEnter(Trigger *Tripped) = 0;
  virtual bool OnExit(Trigger *Tripped) = 0;
};
