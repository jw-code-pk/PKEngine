#pragma once

#include "Entity.h"
#include "GUI.h"
#include "Levels/Level.h"

#include <OgreSceneManager.h>

#include <concepts>
#include <vector>

class World {
public:
  World(Ogre::SceneManager *SceneManager, Ogre::RenderWindow *RenderWindow);
  ~World();

  void LoadLevel(const Ogre::String &Filename);
  void LoadEditorLevel();

  void FrameTick(const float &DeltaTime);
  void UITick(const float &DeltaTime);

  template <std::derived_from<Entity> T> T *CreateEntity() {
    auto node = m_SceneManager->getRootSceneNode()->createChildSceneNode();
    auto entity = new T(node);
    return entity;
  }

  Ogre::Camera *CreateCamera(const Ogre::String &Name);
  void SetActiveCamera(const Ogre::String &Name);

  Ogre::SceneManager *GetSceneManager() { return m_SceneManager; }

  void AddGUI(GUI *GUI);

private:
  Ogre::SceneManager *m_SceneManager;
  Ogre::RenderWindow *m_RenderWindow;

  Level *m_ActiveLevel;

  double m_WorldTime;

  std::vector<GUI *> m_GUIs;
  std::map<Ogre::String, Ogre::Camera *> m_Cameras;
};
