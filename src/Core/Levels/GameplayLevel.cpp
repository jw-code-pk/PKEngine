#include "GameplayLevel.h"

#include "Core/EntityFactory.h"
#include "Core/GEngine.h"

#include <OgreMath.h>
#include <OgreMatrix3.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgrePrerequisites.h>

#include <nlohmann/json.hpp>

#include <cassert>
#include <fstream>

using json = nlohmann::json;

void GameplayLevel::Load(const Ogre::String &Name) {
  CreateResources();

  std::ifstream file(Name);
  if (!file.is_open()) {
    return;
  }

  json levelData;
  file >> levelData;

  for (const auto &item : levelData["entities"]) {
    std::string type = item["type_id"];

    auto entity = SpawnFromTypeId(type);

    if (!entity) {
      continue;
    }

    auto pos = item["pos"];
    entity->GetRoot()->setPosition(pos["x"], pos["y"], pos["z"]);

    auto rot = item["rot"];
    Ogre::Matrix3 rotMat;
    rotMat.FromEulerAnglesYXZ(Ogre::Degree(rot["yaw"]),
                              Ogre::Degree(rot["pitch"]),
                              Ogre::Degree(rot["roll"]));
    entity->GetRoot()->setOrientation(rotMat);

    auto meta = item["meta"];
    entity->SetMetadata(meta);

    if (const auto curve = dynamic_cast<Curve *>(entity)) {
      m_CurveGroup->Register(curve);
    }
  }
}

void GameplayLevel::Unload() { DestroyResources(); }

void GameplayLevel::BeginPlay() {
  m_CurveGroup->RefreshTree();

  Level::BeginPlay();
}

void GameplayLevel::Tick(const float &DeltaTime) { Level::Tick(DeltaTime); }

void GameplayLevel::EndPlay() { Level::EndPlay(); }

void GameplayLevel::CreateResources() {
  m_CurveGroup = new CurveGroup();
  GEngine::Register(m_CurveGroup);
  m_TriggerGroupRegistry = new TriggerGroupRegistry();
  m_TriggerGroupRegistry->Create("Player", Ogre::Vector3::ZERO,
                                 Ogre::Vector3::UNIT_SCALE * 35000);
}

void GameplayLevel::DestroyResources() {
  for (auto entity : m_Entities) {
    delete entity;
  }

  m_Entities.clear();
  m_TickList.clear();

  delete m_CurveGroup;
  delete m_TriggerGroupRegistry;
}

Entity *GameplayLevel::SpawnFromTypeId(const Ogre::String &TypeId) {
  auto entityFactory = GEngine::Get<EntityFactory>();
  auto entity = entityFactory->Spawn(TypeId);

  if (entity) {
    entity->Init();

    m_Entities.push_back(entity);

    if (entity->CanTick()) {
      m_TickList.push_back(entity);
    }
  }

  return entity;
}
