#include "NinjaIdleState.h"
#include "NinjaJumpState.h"
#include "NinjaWalkState.h"

#include "Core/GEngine.h"
#include "Core/Input/InputState.h"
#include "Debug/Ninja.h"
#include "Debug/NinjaSM/NinjaSM.h"

void NinjaIdleState::Enter(Ninja *Target) {
  Target->ChangeAnim("Idle2");
  Target->SetSpeed(0);
}

void NinjaIdleState::Tick(Ninja *Target, const float &DeltaTime) {
  auto inputState = GEngine::Get<InputState>();

  if (Target->IsGrounded() && inputState->Jump.Down) {
    Target->GetStateMachine()->ChangeState<NinjaJumpState>();
  } else if (inputState->HasAxisInput()) {
    Target->GetStateMachine()->ChangeState<NinjaWalkState>();
  }
}

void NinjaIdleState::Exit(Ninja *Target) {}
