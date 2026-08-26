#pragma once

#include <OgreSceneNode.h>
#include <nlohmann/json.hpp>

class Entity {
public:
  Entity(Ogre::SceneNode *Root) { m_Root = Root; }

  virtual ~Entity() = default;

  virtual Ogre::String GetTypeId() = 0;

  virtual bool Init() { return true; }

  virtual void Tick(const float &DeltaTime) {}

  virtual void SetMetadata(const nlohmann::json &Metadata) {}

  virtual nlohmann::json GetMetadata() const { return {}; }

  bool CanTick() { return m_CanTick; }

  Ogre::SceneNode *GetRoot() { return m_Root; }

protected:
  bool m_CanTick;

private:
  Ogre::SceneNode *m_Root;
};
