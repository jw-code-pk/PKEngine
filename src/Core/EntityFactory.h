#pragma once

#include "Entity.h"
#include <vector>

class EntityFactory {
public:
  [[nodiscard]] virtual Entity *Spawn(const Ogre::String &TypeId) = 0;
  virtual std::vector<Ogre::String> GetAvailableEntities() = 0;
};
