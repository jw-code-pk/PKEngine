
#include "World.h"
#include "GEngine.h"

// TODO: cleanup
#include "Debug/DebugLevel.h"

World::World(Ogre::SceneManager *SceneManager,
             Ogre::RenderWindow *RenderWindow) {
  m_SceneManager = SceneManager;
  m_RenderWindow = RenderWindow;
  m_ActiveLevel = nullptr;

  GEngine::Register<World>(this);
}

World::~World() {
  GEngine::Unregister<World>(this);
  m_SceneManager = nullptr;
}

void World::LoadLevel() {
  m_ActiveLevel = new DebugLevel();
  m_ActiveLevel->Init();
}

void World::FrameTick(const float &DeltaTime) {
  if (m_ActiveLevel != nullptr) {
    m_ActiveLevel->Tick(DeltaTime);
  }
}

Ogre::Camera *World::CreateCamera(const std::string &Id) {
  auto cam = m_SceneManager->createCamera(Id);
  m_RenderWindow->addViewport(cam);
  return cam;
}
