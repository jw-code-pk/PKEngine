#include "DebugLevel.h"
#include "Core/GEngine.h"
#include "Core/ResourceLoader.h"
#include "Core/World.h"
#include "Cube.h"
#include "Debug/DebugGUI.h"
#include "Ninja.h"

#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgrePrerequisites.h>

#include <fstream>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <cassert>

using json = nlohmann::json;

void DebugLevel::Init() {
  World *world = nullptr;
  const auto bHasWorld = GEngine::TryGet(world);
  assert(bHasWorld && "No world is registered.");

  auto ninja = world->CreateEntity<Ninja>();
  ninja->Init();

  if (ninja->CanTick()) {
    m_TickList.push_back(ninja);
  }
  m_Entities.push_back(ninja);

  auto cube = world->CreateEntity<Cube>();
  cube->Init();
  m_Entities.push_back(cube);

  m_GUI = new DebugGUI(this);
  world->AddGUI(m_GUI);

  m_CurrentIndex = -1;

  DisplayTestImage();
}

void DebugLevel::Tick(const float &DeltaTime) {
  for (auto entity : m_TickList) {
    entity->Tick(DeltaTime);
  }
}

void DebugLevel::SelectNextEntity() {
  const auto num = m_Entities.size();

  if (num <= 0) {
    return;
  }

  if (m_CurrentIndex >= 0) {
    m_Entities[m_CurrentIndex]->GetRoot()->showBoundingBox(false);
  }

  m_CurrentIndex = (m_CurrentIndex + 1) % num;
  m_Entities[m_CurrentIndex]->GetRoot()->showBoundingBox(true);
}

void DebugLevel::SelectPrevEntity() {
  const auto num = m_Entities.size();

  if (num <= 0) {
    return;
  }

  if (m_CurrentIndex >= 0) {
    m_Entities[m_CurrentIndex]->GetRoot()->showBoundingBox(false);
  }

  m_CurrentIndex = std::max(0, m_CurrentIndex - 1);
  m_Entities[m_CurrentIndex]->GetRoot()->showBoundingBox(true);
}

void DebugLevel::SetEntityPosition(const Ogre::Vector3 &Position) {
  if (m_CurrentIndex < 0) {
    return;
  }

  m_Entities[m_CurrentIndex]->GetRoot()->setPosition(Position);
}

void DebugLevel::SaveLevel(const Ogre::String &Name) {
  json levelData;
  levelData["entities"] = json::array();

  for (auto entity : m_Entities) {
    json entityData;
    entityData["type_id"] = entity->GetTypeId();

    auto pos = entity->GetRoot()->getPosition();
    auto rot = entity->GetRoot()->getOrientation();

    Ogre::Radian yaw, pitch, roll;
    Ogre::Matrix3 rotMat;

    rot.ToRotationMatrix(rotMat);
    rotMat.ToEulerAnglesYXZ(yaw, pitch, roll);

    entityData["pos"] = {{"x", pos.x}, {"y", pos.y}, {"z", pos.z}};

    entityData["rot"] = {{"pitch", pitch.valueDegrees()},
                         {"yaw", yaw.valueDegrees()},
                         {"roll", roll.valueDegrees()}};

    levelData["entities"].push_back(entityData);
  }

  std::ofstream file(Name);
  if (file.is_open()) {
    file << levelData.dump(4);
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
