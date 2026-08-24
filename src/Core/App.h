#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <OgreSceneManager.h>

#include "World.h"

class App : public OgreBites::ApplicationContext,
            public OgreBites::InputListener {
public:
  App() : OgreBites::ApplicationContext("MyOgreGame") {}

  // Ogre App Context
  void setup() override;
  bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
  bool keyPressed(const OgreBites::KeyboardEvent &evt) override;
  bool keyReleased(const OgreBites::KeyboardEvent &evt) override;
  bool mouseMoved(const OgreBites::MouseMotionEvent &evt) override;

  Ogre::SceneManager *GetSceneManager() const;

private:
  World *m_World;
  Ogre::SceneManager *m_SceneManager;
};
