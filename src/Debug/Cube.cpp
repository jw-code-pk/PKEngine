#include "Cube.h"
#include "Core/Entity.h"
#include "Core/GEngine.h"
#include "Core/World.h"

Cube::Cube(Ogre::SceneNode *Root) : Entity(Root) { m_CanTick = false; }

bool Cube::Init() {
  World *world = GEngine::Get<World>();
  auto sceneManager = world->GetSceneManager();

  Ogre::Entity *cubeMesh = sceneManager->createEntity("Cube.mesh");
  Ogre::SceneNode *cubeNode = GetRoot()->createChildSceneNode();
  cubeNode->attachObject(cubeMesh);
  cubeNode->setInheritScale(false);
  cubeNode->setScale(50, 50, 50);

  GetRoot()->setPosition(0, 0, 0);

  return true;
}
