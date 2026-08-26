#pragma once

#include "InputState.h"

#include <OgreInput.h>
#include <unordered_map>

class InputManager {
public:
  InputManager();
  ~InputManager();

  void LoadMappings();

  bool OnKeyDown(const OgreBites::KeyboardEvent &Evt);
  bool OnKeyUp(const OgreBites::KeyboardEvent &Evt);

  inline bool HasMappingOn(const OgreBites::Keycode &Key) {
    return m_KeyMappings.contains(Key);
  }

private:
  std::unordered_map<OgreBites::Keycode, InputAction> m_KeyMappings;
  InputState *m_InputState;
};
