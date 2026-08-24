#include "App.h"
#include "Core/ResourceLoader.h"
#include <OgreApplicationContextBase.h>
#include <OgreFrameListener.h>
#include <OgreImGuiInputListener.h>
#include <OgreImGuiOverlay.h>
#include <OgreTrays.h>
#include <imgui.h>

#include <cassert>
#include <memory>

void App::setup() {
  OgreBites::ApplicationContext::setup();
  addInputListener(this);

  Ogre::Root *root = getRoot();
  m_SceneManager = root->createSceneManager();

  Ogre::RTShader::ShaderGenerator *shadergen =
      Ogre::RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(m_SceneManager);

  root->addFrameListener(this);

  m_SceneManager->addRenderQueueListener(mOverlaySystem);

  m_TrayManager =
      new OgreBites::TrayManager("UIDebug", getRenderWindow(), this);
  m_TrayManager->showFrameStats(OgreBites::TL_BOTTOMRIGHT);
  m_TrayManager->hideCursor();

  m_ResourceLoader = new ResourceLoader();
  m_ResourceLoader->LoadConfig();

  m_World = new World(m_SceneManager, getRenderWindow());
  m_World->LoadLevel();

  // TODO: move these to members

  Ogre::ImGuiOverlay *imguiOverlay = new Ogre::ImGuiOverlay();
  imguiOverlay->setZOrder(300);
  imguiOverlay->show();
  Ogre::OverlayManager::getSingleton().addOverlay(imguiOverlay);

  auto imguiInputListener = new OgreBites::ImGuiInputListener();
  addInputListener(imguiInputListener);
}

void App::shutdown() {
  m_SceneManager->clearScene();

  delete m_World;
  delete m_ResourceLoader;

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
  return false;
}

bool App::keyReleased(const OgreBites::KeyboardEvent &evt) { return true; }

bool App::mouseMoved(const OgreBites::MouseMotionEvent &evt) { return true; }

Ogre::SceneManager *App::GetSceneManager() const { return m_SceneManager; }
