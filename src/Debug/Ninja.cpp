#include "Ninja.h"
#include "Core/Curves/CurveGroup.h"
#include "Core/GEngine.h"
#include "Core/World.h"
#include <OgreMath.h>
#include <cassert>

Ninja::Ninja(Ogre::SceneNode *Root) : CurveFollower(Root) { m_CanTick = true; }

bool Ninja::Init() {
  World *world = nullptr;
  const auto bHasWorld = GEngine::TryGet(world);
  assert(bHasWorld && "No world is registered.");

  auto m_SceneManager = world->GetSceneManager();

  Ogre::Entity *ninjaMesh = m_SceneManager->createEntity("ninja.mesh");
  m_PawnNode = GetRoot()->createChildSceneNode();
  m_PawnNode->attachObject(ninjaMesh);

  GetRoot()->setPosition(0, 0, 0);

  return true;
}

void Ninja::Tick(const float &DeltaTime) {
  assert(m_PawnNode && "Pawn node should be initialised.");

  if (!CurveFollower::HasCurve()) {
    // TODO: This probably needs to move somewhere else (maybe CurveFollower?)
    CurveGroup *curveGroup = nullptr;
    const auto bHasCurveGroup = GEngine::TryGet(curveGroup);
    assert(bHasCurveGroup && "No curve group registered.");

    Curve *curve;
    auto pos = GetRoot()->getPosition();
    if (curveGroup->TryGetClosest(pos, curve)) {
      CurveFollower::Follow(curve, 200, 0);
    }
  }

  CurveFollower::Tick(DeltaTime);

  m_Rotation += DeltaTime * 200.0f;
  const auto targetRot =
      Ogre::Quaternion(Ogre::Degree(m_Rotation), Ogre::Vector3::UNIT_Y);
  m_PawnNode->setOrientation(targetRot);
}
