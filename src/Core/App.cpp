#include "App.h"
#include <OgreSceneManager.h>
#include <cassert>

void App::setup() {
  OgreBites::ApplicationContext::setup();
  addInputListener(this);

  Ogre::Root *root = getRoot();
  m_SceneManager = root->createSceneManager();

  Ogre::RTShader::ShaderGenerator *shadergen =
      Ogre::RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(m_SceneManager);

  root->addFrameListener(this);

  m_World = new World(m_SceneManager, getRenderWindow());
  m_World->LoadLevel();
}

bool App::frameRenderingQueued(const Ogre::FrameEvent &evt) {
  assert(m_World && "Can't tick a frame without a world active.");

  const auto deltaTime = evt.timeSinceLastEvent;
  m_World->FrameTick(deltaTime);
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
