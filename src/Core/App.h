#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <OgreFrameListener.h>
#include <OgrePrerequisites.h>
#include <OgreSceneManager.h>
#include <OgreTrays.h>

#include "ResourceLoader.h"
#include "World.h"

class App : public OgreBites::ApplicationContext,
            public OgreBites::InputListener,
            public OgreBites::TrayListener {
public:
  App() : OgreBites::ApplicationContext("MyOgreGame") {}

  // Ogre App Context
  void setup() override;
  void shutdown() override;

  bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
  bool frameStarted(const Ogre::FrameEvent &evt) override;
  bool keyPressed(const OgreBites::KeyboardEvent &evt) override;
  bool keyReleased(const OgreBites::KeyboardEvent &evt) override;
  bool mouseMoved(const OgreBites::MouseMotionEvent &evt) override;

  // App public interface
  Ogre::SceneManager *GetSceneManager() const;

private:
  World *m_World;
  ResourceLoader *m_ResourceLoader;
  Ogre::SceneManager *m_SceneManager;
  OgreBites::TrayManager *m_TrayManager;
};
