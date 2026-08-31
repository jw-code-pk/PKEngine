#pragma once

#include "TriggerGroup.h"

class TriggerGroupRegistry {
public:
  TriggerGroupRegistry();
  ~TriggerGroupRegistry();

  void Create(const Ogre::String &Name, const Ogre::Vector3 &Origin,
              const Ogre::Vector3 &Extents);
  TriggerGroup *Get(const Ogre::String &Name);

private:
  std::map<Ogre::String, TriggerGroup *> m_Groups;
};
