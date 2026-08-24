#pragma once

#include <OgreSceneNode.h>

class Entity {
public:
  Entity(Ogre::SceneNode *Root) { m_Root = Root; }

  virtual Ogre::String GetTypeId() = 0;

  virtual bool Init() { return true; }

  virtual void Tick(const float &DeltaTime) {}

  bool CanTick() { return m_CanTick; }

  Ogre::SceneNode *GetRoot() { return m_Root; }

protected:
  bool m_CanTick;

private:
  Ogre::SceneNode *m_Root;
};
