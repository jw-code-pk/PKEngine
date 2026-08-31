#include "TriggerSweeper.h"

#include "Trigger.h"
#include "TriggerGroup.h"
#include "TriggerProbe.h"

TriggerSweeper::TriggerSweeper(TriggerGroup *Group) { m_Group = Group; }

TriggerSweeper::~TriggerSweeper() {
  m_QuerySet.clear();
  m_ActiveSet.clear();
}

void TriggerSweeper::RunSweep(TriggerProbe *Probe) {
  for (auto trigger : m_ActiveSet) {
    if (!trigger->CheckOverlap(Probe)) {
      trigger->OnExit(Probe);
      Probe->OnExit(trigger);
    }
  }

  m_QuerySet.clear();
  m_Group->Fetch(Probe, m_QuerySet);

  for (auto trigger : m_QuerySet) {
    if (!m_ActiveSet.contains(trigger)) {
      trigger->OnEnter(Probe);
      Probe->OnEnter(trigger);
    }
  }

  m_ActiveSet.clear();

  for (auto trigger : m_QuerySet) {
    m_ActiveSet.insert(trigger);
  }
}
