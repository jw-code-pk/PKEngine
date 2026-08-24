#pragma once

#include <concepts>

#include "Ogre.h"
#include <OgreSceneManager.h>

#include "Entity.h"
#include "Level.h"

class World {
public:
  World(Ogre::SceneManager *SceneManager, Ogre::RenderWindow *RenderWindow);
  ~World();

  void LoadLevel();
  void FrameTick(const float &DeltaTime);

  template <std::derived_from<Entity> T> T *CreateEntity() {
    auto node = m_SceneManager->getRootSceneNode()->createChildSceneNode();
    auto entity = new T(node);
    return entity;
  }

  Ogre::Camera *CreateCamera(const std::string &Id);

  Ogre::SceneManager *GetSceneManager() { return m_SceneManager; }

private:
  Ogre::SceneManager *m_SceneManager;
  Ogre::RenderWindow *m_RenderWindow;

  Level *m_ActiveLevel;

  double m_WorldTime;
};
