#include "App.h"
#include "Core/ResourceLoader.h"
#include <OgreApplicationContextBase.h>
#include <OgreConfigDialog.h>
#include <OgreFrameListener.h>
#include <OgreTrays.h>
#include <imgui.h>

#include <cassert>

void App::setup() {
  OgreBites::ApplicationContext::setup();
  addInputListener(this);

  Ogre::Root *root = getRoot();
  root->getRenderSystem()->setConfigOption("FSAA", "8");
  root->addFrameListener(this);

  m_SceneManager = root->createSceneManager();
  m_SceneManager->addRenderQueueListener(mOverlaySystem);

  auto shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(m_SceneManager);

  m_TrayManager =
      new OgreBites::TrayManager("UIDebug", getRenderWindow(), this);
  m_TrayManager->showFrameStats(OgreBites::TL_BOTTOMRIGHT);
  m_TrayManager->hideCursor();

  m_ResourceLoader = new ResourceLoader();
  m_ResourceLoader->LoadConfig();

  m_InputManager = new InputManager();
  m_InputManager->LoadMappings();

  m_World = new World(m_SceneManager, getRenderWindow());
  m_World->LoadEditorLevel();

  m_ImGuiOverlay = new Ogre::ImGuiOverlay();
  m_ImGuiOverlay->setZOrder(300);
  m_ImGuiOverlay->show();

  Ogre::OverlayManager::getSingleton().addOverlay(m_ImGuiOverlay);
  m_ImGuiInputListener = new OgreBites::ImGuiInputListener();
  addInputListener(m_ImGuiInputListener);
}

void App::shutdown() {
  m_SceneManager->clearScene();

  delete m_World;
  delete m_ResourceLoader;
  delete m_InputManager;

  Ogre::OverlayManager::getSingleton().destroy(m_ImGuiOverlay);
  removeInputListener(m_ImGuiInputListener);
  delete m_ImGuiInputListener;

  OgreBites::ApplicationContext::shutdown();
}

bool App::frameRenderingQueued(const Ogre::FrameEvent &evt) {
  OgreBites::ApplicationContext::frameRenderingQueued(evt);

  assert(m_World && "Can't tick a frame without a world active.");

  const auto deltaTime = evt.timeSinceLastEvent;
  m_World->FrameTick(deltaTime);
  return true;
}

bool App::frameStarted(const Ogre::FrameEvent &evt) {
  OgreBites::ApplicationContext::frameStarted(evt);

  assert(m_World && "Can't tick a frame without a world active.");

  Ogre::ImGuiOverlay::NewFrame();

  const auto deltaTime = evt.timeSinceLastEvent;
  m_World->UITick(deltaTime);

  m_TrayManager->frameRendered(evt);

  return true;
}

bool App::keyPressed(const OgreBites::KeyboardEvent &evt) {
  if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
    getRoot()->queueEndRendering();
    return true;
  }

  return m_InputManager->OnKeyDown(evt);
}

bool App::keyReleased(const OgreBites::KeyboardEvent &evt) {
  return m_InputManager->OnKeyUp(evt);
}

Ogre::SceneManager *App::GetSceneManager() const { return m_SceneManager; }
