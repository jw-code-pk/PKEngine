#include "NinjaWalkState.h"
#include "NinjaIdleState.h"
#include "NinjaJumpState.h"

#include "Core/GEngine.h"
#include "Core/Input/InputState.h"
#include "Debug/Ninja.h"

void NinjaWalkState::Enter(Ninja *Target) { Target->ChangeAnim("Walk"); }

void NinjaWalkState::Tick(Ninja *Target, const float &DeltaTime) {
  auto inputState = GEngine::Get<InputState>();

  auto speed = inputState->Axis.x * 600;
  Target->SetSpeed(speed);

  if (speed > 0) {
    Target->FaceDirection(Ogre::Vector3::UNIT_X);
  } else if (speed < 0) {
    Target->FaceDirection(-1 * Ogre::Vector3::UNIT_X);
  }

  if (Target->IsGrounded() && inputState->Jump.Down) {
    Target->GetStateMachine()->ChangeState<NinjaJumpState>();
  } else if (!inputState->HasAxisInput()) {
    Target->GetStateMachine()->ChangeState<NinjaIdleState>();
  }
}

void NinjaWalkState::Exit(Ninja *Target) {}
