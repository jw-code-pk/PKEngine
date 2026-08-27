#pragma once

#include "Debug/NinjaSM/NinjaSM.h"

class NinjaIdleState : public NinjaSM::State {

public:
  virtual void Enter(Ninja *Target) override;
  virtual void Tick(Ninja *Target, const float &DeltaTime) override;
  virtual void Exit(Ninja *Target) override;
};
