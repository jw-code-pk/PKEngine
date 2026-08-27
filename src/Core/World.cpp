
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
  if (m_ActiveLevel != nullptr) {
    m_ActiveLevel->Cleanup();
    delete m_ActiveLevel;
  }

  GEngine::Unregister<World>(this);
  m_GUIs.clear();
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

void World::UITick(const float &DeltaTime) {
  for (auto gui : m_GUIs) {
    gui->Tick(DeltaTime);
  }
}

Ogre::Camera *World::CreateCamera(const Ogre::String &Name) {
  assert(!m_Cameras.contains(Name) &&
         std::format("Camera {} already exists.", Name).c_str());

  auto cam = m_SceneManager->createCamera(Name);
  m_Cameras[Name] = cam;

  return cam;
}

void World::SetActiveCamera(const Ogre::String &Name) {
  auto cam = m_Cameras[Name];

  if (cam) {
    m_RenderWindow->removeAllViewports();
    auto vp = m_RenderWindow->addViewport(cam);

    Ogre::Real aspectRatio =
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight());
    cam->setAspectRatio(aspectRatio);
  }
}

void World::AddGUI(GUI *GUI) { m_GUIs.push_back(GUI); }
