#include "EditorLevel.h"
#include "EditorGUI.h"

#include "Core/EntityFactory.h"
#include "Core/GEngine.h"
#include "Core/ResourceLoader.h"
#include "Core/World.h"

#include <OgreMath.h>
#include <OgreMatrix3.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgrePrerequisites.h>

#include <fstream>
#include <nlohmann/json.hpp>

#include <cassert>

using json = nlohmann::json;

void EditorLevel::CreateResources() {
  GameplayLevel::CreateResources();

  // Scene setup

  auto world = GEngine::Get<World>();
  auto sceneManager = world->GetSceneManager();

  // Environment & Lighting

  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  sceneManager->setSkyBox(true, "Examples/SpaceSkyBox", 3000.0f);

  Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -1);
  Ogre::MeshManager::getSingleton().createPlane(
      "GroundPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      plane, 10000, 10000, 20, 20, true, 1, 50, 50, Ogre::Vector3::UNIT_Z);

  Ogre::Entity *groundEntity = sceneManager->createEntity("GroundPlane");
  sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(
      groundEntity);
  groundEntity->setMaterialName("Examples/GrassFloor");

  sceneManager->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(0.0, 0.0, 0.0), 1,
                       500, 10000);

  // Camera

  m_Camera = sceneManager->getRootSceneNode()->createChildSceneNode();
  Ogre::Camera *cam = world->CreateCamera("EditorCam");
  world->SetActiveCamera("EditorCam");

  cam->setNearClipDistance(5);
  m_Camera->attachObject(cam);
  m_Camera->setPosition(0, 0, 1500);
  m_CameraDistance = 1500;
  m_CameraDirection = Ogre::Vector3::UNIT_Z;

  // Gameplay bits

  m_CurveTreeVisualiser = world->CreateEntity<OctreeVisualiser<Curve *>>();

  // Editor bits

  m_GUI = new EditorGUI(this);
  world->AddGUI(m_GUI);

  m_CurrentIndex = -1;

  DisplayTestImage();
}

void EditorLevel::DestroyResources() { GameplayLevel::DestroyResources(); }

void EditorLevel::BeginPlay() { GameplayLevel::BeginPlay(); }

void EditorLevel::EndPlay() {
  GameplayLevel::EndPlay();

  auto world = GEngine::Get<World>();
  world->SetActiveCamera("EditorCam");
}

void EditorLevel::SelectNextEntity() {
  const auto num = m_Entities.size();

  if (num <= 0) {
    return;
  }

  if (m_CurrentIndex >= 0) {
    m_Entities[m_CurrentIndex]->GetRoot()->showBoundingBox(false);
  }

  m_CurrentIndex = (m_CurrentIndex + 1) % num;
  m_Entities[m_CurrentIndex]->GetRoot()->showBoundingBox(true);

  CameraToEntity();
}

void EditorLevel::SelectPrevEntity() {
  const auto num = m_Entities.size();

  if (num <= 0) {
    return;
  }

  if (m_CurrentIndex >= 0) {
    m_Entities[m_CurrentIndex]->GetRoot()->showBoundingBox(false);
  }

  m_CurrentIndex = m_CurrentIndex - 1;

  if (m_CurrentIndex < 0) {
    m_CurrentIndex = num - 1;
  }

  m_Entities[m_CurrentIndex]->GetRoot()->showBoundingBox(true);

  CameraToEntity();
}

Ogre::Vector3 EditorLevel::GetEntityPosition() {
  if (m_CurrentIndex < 0) {
    return Ogre::Vector3::ZERO;
  }

  return m_Entities[m_CurrentIndex]->GetRoot()->getPosition();
}

void EditorLevel::SetEntityPosition(const Ogre::Vector3 &Position) {
  if (m_CurrentIndex < 0) {
    return;
  }

  m_Entities[m_CurrentIndex]->GetRoot()->setPosition(Position);
}

Ogre::Vector3 EditorLevel::GetEntityRotation() {
  if (m_CurrentIndex < 0) {
    return Ogre::Vector3::ZERO;
  }

  auto entity = m_Entities[m_CurrentIndex];
  auto rot = entity->GetRoot()->getOrientation();

  Ogre::Radian yaw, pitch, roll;
  Ogre::Matrix3 rotMat;

  rot.ToRotationMatrix(rotMat);
  rotMat.ToEulerAnglesYXZ(yaw, pitch, roll);

  return Ogre::Vector3(pitch.valueDegrees(), yaw.valueDegrees(),
                       roll.valueDegrees());
}

void EditorLevel::SetEntityRotation(const Ogre::Vector3 &Rotation) {
  if (m_CurrentIndex < 0) {
    return;
  }

  Ogre::Matrix3 rotMat;
  Ogre::Degree xRot(Rotation.x), yRot(Rotation.y), zRot(Rotation.z);
  Ogre::Radian yaw(yRot.valueRadians()), pitch(xRot.valueRadians()),
      roll(zRot.valueRadians());

  rotMat.FromEulerAnglesYXZ(yaw, pitch, roll);

  m_Entities[m_CurrentIndex]->GetRoot()->setOrientation(rotMat);
}

Ogre::String EditorLevel::GetEntityTypeId() {
  if (m_CurrentIndex < 0) {
    return Ogre::String();
  }

  return m_Entities[m_CurrentIndex]->GetTypeId();
}

bool EditorLevel::TryGetEntity(Entity *&OutEntity) {
  if (m_CurrentIndex < 0) {
    OutEntity = nullptr;
    return false;
  }

  OutEntity = m_Entities[m_CurrentIndex];
  return true;
}

void EditorLevel::CameraToEntity() {
  if (m_CurrentIndex < 0) {
    return;
  }

  auto entityPos = m_Entities[m_CurrentIndex]->GetRoot()->getPosition();
  auto cameraOffset =
      Ogre::Vector3(m_CameraDirection.x, 1, m_CameraDirection.z) *
      m_CameraDistance;
  auto origin = entityPos + cameraOffset;
  m_Camera->setPosition(origin);

  auto cameraAim = -m_CameraDirection;
  m_Camera->setOrientation(Ogre::Quaternion::IDENTITY);
  m_Camera->setDirection(cameraAim, Ogre::Node::TS_WORLD);

  m_Camera->pitch(Ogre::Radian(-0.5f * Ogre::Math::HALF_PI),
                  Ogre::Node::TS_LOCAL);
}

void EditorLevel::SetCameraDistance(const float &Distance) {
  assert(Distance > 0 && "Camera distance can't be negative");
  m_CameraDistance = Distance;

  CameraToEntity();
}

void EditorLevel::SetCameraDirection(const Ogre::Vector3 &Direction) {
  assert(Direction.squaredLength() > 0 && "Camera direction is not valid.");
  m_CameraDirection = Direction.normalisedCopy();

  CameraToEntity();
}

void EditorLevel::SaveTo(const Ogre::String &Name) {
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

    entityData["meta"] = entity->GetMetadata();

    levelData["entities"].push_back(entityData);
  }

  std::ofstream file(Name);
  if (file.is_open()) {
    file << levelData.dump(4);
  }
}

std::vector<Ogre::String> EditorLevel::GetAvailableEntities() {
  auto entityFactory = GEngine::Get<EntityFactory>();
  return entityFactory->GetAvailableEntities();
}

void EditorLevel::CreateEntity(const Ogre::String &TypeId,
                               const Ogre::Vector3 &Position) {
  auto entity = SpawnFromTypeId(TypeId);

  if (entity) {
    entity->GetRoot()->setPosition(Position);

    if (const auto curve = dynamic_cast<Curve *>(entity)) {
      m_CurveGroup->Register(curve);
    }

    if (m_CurrentIndex >= 0) {
      m_Entities[m_CurrentIndex]->GetRoot()->showBoundingBox(false);
    }

    m_CurrentIndex = m_Entities.size() - 1;

    entity->GetRoot()->showBoundingBox(true);
  }
}

void EditorLevel::DeleteEntity() {
  if (m_CurrentIndex >= 0) {
    m_Entities[m_CurrentIndex]->GetRoot()->showBoundingBox(false);
  }

  auto entity = m_Entities[m_CurrentIndex];
  auto world = GEngine::Get<World>();

  if (const auto curve = dynamic_cast<Curve *>(entity)) {
    m_CurveGroup->Unregister(curve);
  }

  world->GetSceneManager()->destroySceneNode(entity->GetRoot());
  m_Entities.erase(m_Entities.begin() + m_CurrentIndex);
  delete entity;

  if (m_Entities.size() > 0) {
    m_CurrentIndex = m_Entities.size() - 1;
    m_Entities[m_CurrentIndex]->GetRoot()->showBoundingBox(true);
  }
}

void EditorLevel::ShowDebugVisuals(const DebugVisualType &VisualType) {
  switch (VisualType) {
  case DebugVisualType::None:
    m_CurveTreeVisualiser->RemoveGizmos();
    break;
  case DebugVisualType::CurveOctree:
    m_CurveTreeVisualiser->ShowGizmos(m_CurveGroup->GetTree());
    break;
  default:
    break;
  }
}

void EditorLevel::DisplayTestImage() {
  // TODO: decide if this can vbe used for game UI
  auto resourceLoader = GEngine::Get<ResourceLoader>();
  resourceLoader->CreateUIMaterial("UIShared", "UIImageMaterial", "test.png");

  Ogre::OverlayManager &overlayMgr = Ogre::OverlayManager::getSingleton();
  Ogre::OverlayContainer *panel = static_cast<Ogre::OverlayContainer *>(
      overlayMgr.createOverlayElement("Panel", "ImagePanel"));

  overlayMgr.setPixelRatio(1.0f);

  panel->setMetricsMode(Ogre::GMM_RELATIVE_ASPECT_ADJUSTED);
  panel->setPosition(100.0f, 100.0f);
  panel->setDimensions(512.0f, 512.0f);

  panel->setMaterialName("UIImageMaterial");

  Ogre::Overlay *overlay = overlayMgr.create("TestImageOverlay");
  overlay->add2D(panel);
  overlay->show();
}
