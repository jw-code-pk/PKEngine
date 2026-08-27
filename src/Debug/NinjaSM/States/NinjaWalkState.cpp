#include "NinjaWalkState.h"
#include "Core/Curves/CurveGroup.h"
#include "Core/GEngine.h"
#include "Core/Input/InputState.h"
#include "Debug/Ninja.h"
#include "NinjaIdleState.h"

void NinjaWalkState::Enter(Ninja *Target) { Target->ChangeAnim("Walk"); }

void NinjaWalkState::Tick(Ninja *Target, const float &DeltaTime) {
  auto inputState = GEngine::Get<InputState>();

  if (!inputState->HasAxisInput()) {
    Target->GetStateMachine()->ChangeState<NinjaIdleState>();
  }

  if (inputState->HasAxisInput() && Target->IsCoyote()) {
    auto curveGroup = GEngine::Get<CurveGroup>();
    Target->FollowClosest(Target->GetRoot()->getPosition(), curveGroup);
  }

  auto speed = inputState->Axis.x * 600;
  Target->SetSpeed(speed);

  if (speed > 0) {
    Target->FaceDirection(Ogre::Vector3::UNIT_X);
  } else if (speed < 0) {
    Target->FaceDirection(-1 * Ogre::Vector3::UNIT_X);
  }
}

void NinjaWalkState::Exit(Ninja *Target) {}
