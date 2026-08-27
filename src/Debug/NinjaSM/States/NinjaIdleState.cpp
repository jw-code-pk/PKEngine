#include "NinjaIdleState.h"
#include "Core/GEngine.h"
#include "Core/Input/InputState.h"
#include "Debug/Ninja.h"
#include "Debug/NinjaSM/NinjaSM.h"
#include "NinjaWalkState.h"

void NinjaIdleState::Enter(Ninja *Target) {
  Target->ChangeAnim("Idle2");
  Target->SetSpeed(0);
}

void NinjaIdleState::Tick(Ninja *Target, const float &DeltaTime) {
  auto inputState = GEngine::Get<InputState>();

  if (inputState->HasAxisInput()) {
    Target->GetStateMachine()->ChangeState<NinjaWalkState>();
  }
}

void NinjaIdleState::Exit(Ninja *Target) {}
