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

  auto inputState = GEngine::Get<InputState>();

  // TODO: This probably needs to move somewhere else (maybe CurveFollower?)
  // also not sure if GEngine is the place for level resources
  auto curveGroup = GEngine::Get<CurveGroup>();
  if (inputState->HasAxisInput() && CurveFollower::IsCoyote()) {
    Curve *curve;
    auto pos = GetRoot()->getPosition();
    if (curveGroup->TryGetClosest(pos, curve,
                                  CurveFollower::CurrentCurveId())) {

      const auto startDistance =
          inputState->Axis.x > 0 ? 0 : curve->GetLength();
      CurveFollower::Follow(curve, 0, startDistance);
    }
  }

  CurveFollower::Tick(DeltaTime);

  auto speed = inputState->Axis.x * 200;

  CurveFollower::SetSpeed(speed);

  // m_Rotation += DeltaTime * 200.0f;
  // const auto targetRot =
  //     Ogre::Quaternion(Ogre::Degree(m_Rotation), Ogre::Vector3::UNIT_Y);
  // m_PawnNode->setOrientation(targetRot);
}
