#pragma once

#include <Ogre.h>

enum class InputAction { Forward, Backward, Jump, Attack };

struct ActionState {
  bool Down;
  bool Hold;
  bool Up;
};

struct InputState {
  Ogre::Vector2 Axis;
  ActionState Jump;
  ActionState Attack;
};
