#include "NinjaJumpState.h"
#include "NinjaIdleState.h"
#include "NinjaWalkState.h"

#include "Core/GEngine.h"
#include "Core/Input/InputState.h"
#include "Debug/Ninja.h"

void NinjaJumpState::Enter(Ninja *Target) {
  Target->ChangeAnim("Idle1");
  Target->Launch(1800);
}

void NinjaJumpState::Tick(Ninja *Target, const float &DeltaTime) {
  auto inputState = GEngine::Get<InputState>();

  if (!inputState->HasAxisInput()) {
    Target->SetSpeed(0);

    if (Target->IsGrounded()) {
      Target->GetStateMachine()->ChangeState<NinjaIdleState>();
    }
  } else {
    auto speed = inputState->Axis.x * 600;
    Target->SetSpeed(speed);

    if (speed > 0) {
      Target->FaceDirection(Ogre::Vector3::UNIT_X);
    } else if (speed < 0) {
      Target->FaceDirection(-1 * Ogre::Vector3::UNIT_X);
    }

    if (Target->IsGrounded()) {
      Target->GetStateMachine()->ChangeState<NinjaWalkState>();
    }
  }
}

void NinjaJumpState::Exit(Ninja *Target) {}
