#include "BounceTrigger.h"

#include "Core/GEngine.h"
#include "Core/Triggers/TriggerGroup.h"
#include "Core/Triggers/TriggerGroupRegistry.h"
#include "Core/Triggers/TriggerProbe.h"
#include "Core/World.h"

BounceTrigger::BounceTrigger(Ogre::SceneNode *Root) : Entity(Root) {}

bool BounceTrigger::Init() {
  auto world = GEngine::Get<World>();
  auto sceneManager = world->GetSceneManager();

  Ogre::Entity *cubeMesh = sceneManager->createEntity("Cube.mesh");
  Ogre::SceneNode *cubeNode = GetRoot()->createChildSceneNode();

  cubeNode->attachObject(cubeMesh);
  cubeNode->setInheritScale(false);
  cubeNode->setScale(50, 50, 50);

  return true;
}

void BounceTrigger::BeginPlay() {
  auto registery = GEngine::Get<TriggerGroupRegistry>();
  auto group = registery->Get("Player");

  // TODO: this needs to move somewhere else - maybe Init()?
  auto origin = GetRoot()->getPosition();
  auto extents = Ogre::Vector3::UNIT_SCALE * 50;
  m_AAB = Ogre::AxisAlignedBox(origin - extents, origin + extents);

  group->Register(this);
}

void BounceTrigger::EndPlay() {
  auto registery = GEngine::Get<TriggerGroupRegistry>();
  auto group = registery->Get("Player");
  group->Unregister(this);
}

bool BounceTrigger::CheckOverlap(TriggerProbe *Probe) {
  return m_AAB.intersects(Probe->GetAAB());
}

void BounceTrigger::OnEnter(TriggerProbe *Probe) {
  GEngine::Log("Probe entered bounce trigger");
}

void BounceTrigger::OnExit(TriggerProbe *Probe) {
  GEngine::Log("Probe exited bounce trigger");
}
