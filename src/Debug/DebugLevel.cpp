#include "DebugLevel.h"
#include "Core/GEngine.h"
#include "Core/ResourceLoader.h"
#include "Core/World.h"
#include "Cube.h"
#include "Ninja.h"

#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>

#include <cassert>

void DebugLevel::Init() {
  World *world = nullptr;
  const auto bHasWorld = GEngine::TryGet(world);

  assert(bHasWorld && "No world is registered.");

  auto ninja = world->CreateEntity<Ninja>();
  ninja->Init();

  if (ninja->CanTick()) {
    m_TickList.push_back(ninja);
  }

  auto cube = world->CreateEntity<Cube>();
  cube->Init();

  DisplayTestImage();
}

void DebugLevel::Tick(const float &DeltaTime) {
  for (auto entity : m_TickList) {
    entity->Tick(DeltaTime);
  }
}

void DebugLevel::DisplayTestImage() {
  // Load resources
  ResourceLoader *resourceLoader = nullptr;
  const auto bHasResourceLoader = GEngine::TryGet(resourceLoader);
  assert(bHasResourceLoader && "No resource loader registered.");
  resourceLoader->CreateUIMaterial("UIShared", "UIImageMaterial", "test.png");

  // Display the image
  Ogre::OverlayManager &overlayMgr = Ogre::OverlayManager::getSingleton();
  Ogre::OverlayContainer *panel = static_cast<Ogre::OverlayContainer *>(
      overlayMgr.createOverlayElement("Panel", "ImagePanel"));

  overlayMgr.setPixelRatio(1.0f);

  panel->setMetricsMode(Ogre::GMM_RELATIVE_ASPECT_ADJUSTED);
  panel->setPosition(100.0f, 100.0f);   // Top-Left at 10% screen width/height
  panel->setDimensions(512.0f, 512.0f); // 30% screen width/height

  panel->setMaterialName("UIImageMaterial");

  Ogre::Overlay *overlay = overlayMgr.create("TestImageOverlay");
  overlay->add2D(panel);
  overlay->show();
}
