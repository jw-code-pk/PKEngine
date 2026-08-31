#pragma once

#include "Core/Octree.h"
#include "Trigger.h"

class TriggerGroup {
public:
  TriggerGroup(const Ogre::String &Name, const Ogre::Vector3 &Origin,
               const Ogre::Vector3 &Extents);
  ~TriggerGroup();

  Ogre::String GetName() { return m_Name; }

  void Fetch(TriggerProbe *Probe, std::set<Trigger *> &Results);

  void Register(Trigger *Trigger);
  void Unregister(Trigger *Trigger);

private:
  Ogre::String m_Name;
  Octree<Trigger *> *m_Triggers;
  std::set<Trigger *> m_QueryResults;
};
