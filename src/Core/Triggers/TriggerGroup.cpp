#include "TriggerGroup.h"
#include "TriggerProbe.h"

TriggerGroup::TriggerGroup(const Ogre::String &Name,
                           const Ogre::Vector3 &Origin,
                           const Ogre::Vector3 &Extents) {
  m_Name = Name;
  m_Triggers = new Octree<Trigger *>(Origin, Extents);
}

TriggerGroup::~TriggerGroup() { delete m_Triggers; }

void TriggerGroup::Fetch(TriggerProbe *Probe, std::set<Trigger *> &Results) {
  m_QueryResults.clear();
  m_Triggers->FetchAll(Probe->GetAAB(), m_QueryResults);

  for (auto result : m_QueryResults) {
    if (result->CheckOverlap(Probe)) {
      Results.insert(result);
    }
  }
}

void TriggerGroup::Register(Trigger *Trig) {
  m_Triggers->TryAdd(Trig->GetOrigin(), Trig);
}

void TriggerGroup::Unregister(Trigger *Trig) {
  m_Triggers->TryRemove(Trig->GetOrigin(), Trig);
}
