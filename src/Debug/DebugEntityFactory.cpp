#include "DebugEntityFactory.h"

#include "Core/Curves/Arc.h"
#include "Core/Curves/Line.h"
#include "Core/GEngine.h"
#include "Core/World.h"

#include "Debug/Cube.h"
#include "Debug/Ninja.h"
#include "Debug/Triggers/BounceTrigger.h"

DebugEntityFactory::DebugEntityFactory() {

  m_AvailableEnities.push_back("Ninja");
  m_AvailableEnities.push_back("Arc");
  m_AvailableEnities.push_back("Line");
  m_AvailableEnities.push_back("Cube");
  m_AvailableEnities.push_back("Bounce");

  GEngine::Register<EntityFactory>(this);
}

DebugEntityFactory::~DebugEntityFactory() {
  GEngine::Unregister<EntityFactory>(this);
}

Entity *DebugEntityFactory::Spawn(const Ogre::String &TypeId) {
  Entity *result = nullptr;

  auto world = GEngine::Get<World>();

  if (TypeId == "Ninja") {
    result = world->CreateEntity<Ninja>();
  } else if (TypeId == "Arc") {
    result = world->CreateEntity<Arc>();
  } else if (TypeId == "Line") {
    result = world->CreateEntity<Line>();
  } else if (TypeId == "Cube") {
    result = world->CreateEntity<Cube>();
  } else if (TypeId == "Bounce") {
    result = world->CreateEntity<BounceTrigger>();
  }

  return result;
}
