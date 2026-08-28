#pragma once

#include "Core/EntityFactory.h"

class DebugEntityFactory : public EntityFactory {
public:
  DebugEntityFactory();
  ~DebugEntityFactory();

  [[nodiscard]] virtual Entity *Spawn(const Ogre::String &TypeId) override;
  std::vector<Ogre::String> GetAvailableEntities() override {
    return m_AvailableEnities;
  };

private:
  std::vector<Ogre::String> m_AvailableEnities;
};
