#include "Ninja.h"
#include "Core/GEngine.h"
#include "Core/World.h"
#include <OgreMath.h>
#include <cassert>

Ninja::Ninja(Ogre::SceneNode *Root) : Entity(Root) { m_CanTick = true; }

bool Ninja::Init() {
  World *world = nullptr;

  const auto bHasWorld = GEngine::TryGet(world);

  assert(bHasWorld && "No world is registered.");

  auto m_SceneManager = world->GetSceneManager();

  Ogre::Light *light = m_SceneManager->createLight("MainLight");
  Ogre::SceneNode *lightNode = GetRoot()->createChildSceneNode();
  lightNode->attachObject(light);
  lightNode->setPosition(120, 180, 150);

  Ogre::SceneNode *camNode = GetRoot()->createChildSceneNode();
  Ogre::Camera *cam = world->CreateCamera("MainCam");
  cam->setNearClipDistance(5);
  camNode->attachObject(cam);
  camNode->setPosition(0, 0, 1500);

  Ogre::Entity *ninjaMesh = m_SceneManager->createEntity("Ninja", "ninja.mesh");
  m_PawnNode = GetRoot()->createChildSceneNode();
  m_PawnNode->attachObject(ninjaMesh);
  m_PawnNode->setPosition(0, -100, 0);

  GetRoot()->setPosition(0, 0, 0);

  return true;
}

void Ninja::Tick(const float &DeltaTime) {
  assert(m_PawnNode && "Pawn node should be initialised.");

  m_Rotation += DeltaTime * 200.0f;
  const auto targetRot =
      Ogre::Quaternion(Ogre::Degree(m_Rotation), Ogre::Vector3::UNIT_Y);
  m_PawnNode->setOrientation(targetRot);
}
