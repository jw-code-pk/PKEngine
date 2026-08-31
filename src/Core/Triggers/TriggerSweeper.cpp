#include "TriggerSweeper.h"

#include "Trigger.h"
#include "TriggerGroup.h"
#include "TriggerProbe.h"

TriggerSweeper::TriggerSweeper(TriggerProbe *Probe, TriggerGroup *Group) {
  m_Probe = Probe;
  m_Group = Group;
}

TriggerSweeper::~TriggerSweeper() {
  m_QuerySet.clear();
  m_ActiveSet.clear();
}

void TriggerSweeper::RunSweep() {
  for (auto trigger : m_ActiveSet) {
    if (!trigger->CheckOverlap(m_Probe)) {
      trigger->OnExit(m_Probe);
      m_Probe->OnExit(trigger);
    }
  }

  m_QuerySet.clear();
  m_Group->Fetch(m_Probe, m_QuerySet);

  for (auto trigger : m_QuerySet) {
    if (!m_ActiveSet.contains(trigger)) {
      trigger->OnEnter(m_Probe);
      m_Probe->OnEnter(trigger);
    }
  }

  m_ActiveSet.clear();

  for (auto trigger : m_QuerySet) {
    m_ActiveSet.insert(trigger);
  }
}
