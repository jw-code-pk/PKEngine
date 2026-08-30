#pragma once

#include "Core/Octree.h"
#include "Trigger.h"

class TriggerGroup {
public:
  TriggerGroup();
  ~TriggerGroup();

  void FetchOverlapped(const Ogre::Vector3 &Point,
                       std::vector<Trigger *> &Results);

  void Register(Trigger *Trigger);
  void Unregister(Trigger *Trigger);

private:
  Octree<Trigger *> *m_Triggers;
  std::vector<Trigger *> m_QueryResults;
};
