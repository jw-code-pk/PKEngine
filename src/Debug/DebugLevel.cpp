#include "DebugLevel.h"
#include "Core/GEngine.h"
#include "Core/World.h"
#include "Ninja.h"

void DebugLevel::Init() {
  World *world = nullptr;
  const auto bHasWorld = GEngine::TryGet(world);

  assert(bHasWorld && "No world is registered.");

  auto ninja = world->CreateEntity<Ninja>();
  ninja->Init();

  if (ninja->CanTick()) {
    m_TickList.push_back(ninja);
  }
}

void DebugLevel::Tick(const float &DeltaTime) {
  for (auto entity : m_TickList) {
    entity->Tick(DeltaTime);
  }
}
