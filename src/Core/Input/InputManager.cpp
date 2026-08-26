#include "InputManager.h"
#include "Core/GEngine.h"

InputManager::InputManager() {
  m_InputState = new InputState();
  GEngine::Register<InputState>(m_InputState);
}

InputManager::~InputManager() {
  GEngine::Unregister<InputState>(m_InputState);
  delete m_InputState;
}

void InputManager::LoadMappings() {
  m_KeyMappings['a'] = InputAction::Backward;
  m_KeyMappings['d'] = InputAction::Forward;
  m_KeyMappings['j'] = InputAction::Attack;
  m_KeyMappings[OgreBites::SDLK_SPACE] = InputAction::Jump;
}

bool InputManager::OnKeyDown(const OgreBites::KeyboardEvent &Evt) {
  if (!HasMappingOn(Evt.keysym.sym)) {
    return false;
  }

  const auto action = m_KeyMappings[Evt.keysym.sym];

  switch (action) {
  case InputAction::Forward:
    m_InputState->Axis.x = 1;
    break;
  case InputAction::Backward:
    m_InputState->Axis.x = -1;
    break;
  case InputAction::Attack:
    m_InputState->Attack.Down = true;
    break;
  case InputAction::Jump:
    m_InputState->Jump.Down = true;
    break;
  }

  return true;
}

bool InputManager::OnKeyUp(const OgreBites::KeyboardEvent &Evt) {
  if (!HasMappingOn(Evt.keysym.sym)) {
    return false;
  }

  const auto action = m_KeyMappings[Evt.keysym.sym];

  switch (action) {
  case InputAction::Forward:
    if (m_InputState->Axis.x > 0) {
      m_InputState->Axis.x = 0;
    }
    break;
  case InputAction::Backward:
    if (m_InputState->Axis.x < 0) {
      m_InputState->Axis.x = 0;
    }
    break;
  case InputAction::Attack:
    m_InputState->Attack.Down = false;
    break;
  case InputAction::Jump:
    m_InputState->Jump.Down = false;
    break;
  }

  return true;
}
