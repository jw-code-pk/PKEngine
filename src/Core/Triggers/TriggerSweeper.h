#pragma once

#include "Trigger.h"
#include "TriggerGroup.h"
#include <Ogre.h>

class TriggerSweeper {
public:
  TriggerSweeper(TriggerGroup *Group);
  ~TriggerSweeper();

  void RunSweep(TriggerProbe *Probe);

private:
  TriggerGroup *m_Group;
  std::set<Trigger *> m_ActiveSet;
  std::set<Trigger *> m_QuerySet;
};
