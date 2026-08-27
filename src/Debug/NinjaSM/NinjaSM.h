#pragma once

#include "Core/SM/StateMachine.h"

class Ninja;

class NinjaSM : public StateMachine<Ninja> {
public:
  NinjaSM(Ninja *Target) : StateMachine<Ninja>(Target) {};
};
