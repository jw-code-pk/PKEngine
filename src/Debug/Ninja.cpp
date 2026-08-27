#include "Ninja.h"
#include "Core/Curves/CurveGroup.h"
#include "Core/GEngine.h"
#include "Core/Input/InputState.h"
#include "Core/World.h"
#include <OgreMath.h>
#include <cassert>

Ninja::Ninja(Ogre::SceneNode *Root) : CurveFollower(Root) { m_CanTick = true; }

bool Ninja::Init() {
  auto world = GEngine::Get<World>();
  auto m_SceneManager = world->GetSceneManager();

  Ogre::Entity *ninjaMesh = m_SceneManager->createEntity("ninja.mesh");
  m_PawnNode = GetRoot()->createChildSceneNode();
  m_PawnNode->attachObject(ninjaMesh);

  GetRoot()->setPosition(0, 0, 0);

  return true;
}

void Ninja::Tick(const float &DeltaTime) {
  assert(m_PawnNode && "Pawn node should be initialised.");

  CurveFollower::Tick(DeltaTime);

  auto inputState = GEngine::Get<InputState>();

  if (inputState->HasAxisInput() && CurveFollower::IsCoyote()) {
    auto curveGroup = GEngine::Get<CurveGroup>();
    FollowClosest(GetRoot()->getPosition(), curveGroup);
  }

  auto speed = inputState->Axis.x * 200;
  CurveFollower::SetSpeed(speed);

  // m_Rotation += DeltaTime * 200.0f;
  // const auto targetRot =
  //     Ogre::Quaternion(Ogre::Degree(m_Rotation), Ogre::Vector3::UNIT_Y);
  // m_PawnNode->setOrientation(targetRot);
}
