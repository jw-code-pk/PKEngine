#pragma once

#include "Trigger.h"
#include "TriggerGroup.h"
#include <Ogre.h>

class TriggerSweeper {
public:
  TriggerSweeper(TriggerProbe *Probe, TriggerGroup *Group);
  ~TriggerSweeper();

  void RunSweep();

private:
  TriggerGroup *m_Group;
  TriggerProbe *m_Probe;

  std::set<Trigger *> m_ActiveSet;
  std::set<Trigger *> m_QuerySet;
};
