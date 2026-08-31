#include "TriggerGroupRegistry.h"
#include "Core/GEngine.h"

TriggerGroupRegistry::TriggerGroupRegistry() { GEngine::Register(this); }

TriggerGroupRegistry::~TriggerGroupRegistry() {
  GEngine::Unregister(this);
  for (auto group : m_Groups) {
    delete group.second;
  }
  m_Groups.clear();
}

void TriggerGroupRegistry::Create(const Ogre::String &Name,
                                  const Ogre::Vector3 &Origin,
                                  const Ogre::Vector3 &Extents) {
  assert(!m_Groups.contains(Name) && "Trigger group already exists.");

  auto group = new TriggerGroup(Name, Origin, Extents);
  m_Groups[Name] = group;
}

TriggerGroup *TriggerGroupRegistry::Get(const Ogre::String &Name) {
  assert(m_Groups.contains(Name) && "Trigger group doesn't exist.");

  return m_Groups[Name];
}
