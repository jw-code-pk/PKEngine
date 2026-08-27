#include "Level.h"

void Level::BeginPlay() {
  for (auto entity : m_Entities) {
    entity->BeginPlay();
  }
}

void Level::Tick(const float &DeltaTime) {
  if (m_IsTickEnabled) {
    for (auto tickable : m_TickList) {
      tickable->Tick(DeltaTime);
    }
  }
}

void Level::EndPlay() {
  for (auto entity : m_Entities) {
    entity->EndPlay();
  }
}
