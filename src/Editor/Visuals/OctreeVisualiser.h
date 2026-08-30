#pragma once

#include "Core/Entity.h"
#include "Core/GEngine.h"
#include "Core/Octree.h"
#include "Core/World.h"

#include <OgreWireBoundingBox.h>

template <typename T> class OctreeVisualiser : public Entity {
public:
  virtual Ogre::String GetTypeId() override {
    return typeid(OctreeVisualiser<T>).name();
  }

  OctreeVisualiser(Ogre::SceneNode *Root) : Entity(Root) {}

  virtual ~OctreeVisualiser() override {
    for (auto box : m_Boxes) {
      delete box;
    }
    m_Boxes.clear();
  }

  void ShowGizmos(Octree<T> *Tree) {
    for (auto box : m_Boxes) {
      delete box;
    }
    m_Boxes.clear();

    GetRoot()->removeAndDestroyAllChildren();
    CreateOctreeGizmo(GetRoot(), Tree);

    std::vector<Octree<T> *> children;
    Tree->GetChildren(children);

    for (auto child : children) {
      CreateOctreeGizmo(GetRoot(), child);
    }
  }

protected:
  void CreateOctreeGizmo(Ogre::SceneNode *Root, Octree<T> *Tree) {
    auto world = GEngine::Get<World>();
    auto sceneManager = world->GetSceneManager();

    auto mat = Ogre::MaterialManager::getSingleton().getByName(
        "BaseWhiteNoLighting", Ogre::RGN_INTERNAL);
    auto box = new Ogre::WireBoundingBox();
    m_Boxes.push_back(box);
    box->setupBoundingBox(Tree->GetAABB());
    box->setMaterial(mat);

    auto node = Root->createChildSceneNode();
    node->attachObject(box);
  }

private:
  std::vector<Ogre::WireBoundingBox *> m_Boxes;
};
