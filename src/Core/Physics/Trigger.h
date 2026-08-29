#pragma once

#include <Ogre.h>

class Trigger {
public:
  virtual bool CheckOverlap(const Ogre::Vector3 &Point) = 0;
  virtual bool CheckOverlap(const Trigger *Other) = 0;
};
