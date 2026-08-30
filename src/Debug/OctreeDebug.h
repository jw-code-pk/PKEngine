#pragma once

#include "Core/GEngine.h"
#include "Core/Octree.h"
#include "Core/World.h"
#include <OgreWireBoundingBox.h>

template <typename T> class OctreeDebug {
public:
  static void ShowGizmos(Ogre::SceneNode *Root, Octree<T> *Tree) {
    CreateOctreeGizmo(Root, Tree);

    std::vector<Octree<T> *> children;
    Tree->GetChildren(children);

    for (auto child : children) {
      CreateOctreeGizmo(Root, child);
    }
  }

private:
  static void CreateOctreeGizmo(Ogre::SceneNode *Root, Octree<T> *Tree) {
    auto world = GEngine::Get<World>();
    auto sceneManager = world->GetSceneManager();

    auto mat = Ogre::MaterialManager::getSingleton().getByName(
        "BaseWhiteNoLighting", Ogre::RGN_INTERNAL);
    auto box = new Ogre::WireBoundingBox();
    box->setupBoundingBox(Tree->GetAABB());
    box->setMaterial(mat);

    auto node = Root->createChildSceneNode();
    node->attachObject(box);
  }
};
