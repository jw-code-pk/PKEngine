#include "DebugGUI.h"
#include "Debug/DebugLevel.h"
#include "imgui.h"
#include <OgrePrerequisites.h>

DebugGUI::DebugGUI(DebugLevel *Level) {
  m_Level = Level;
  m_Position = Ogre::Vector3::ZERO;
}

void DebugGUI::Tick(const float &DeltaTime) {
  ImGui::Begin("Debug");

  if (ImGui::Button("Add Cube")) {
  }

  if (ImGui::Button("Next")) {
    m_Level->SelectNextEntity();
  }

  if (ImGui::Button("Prev")) {
    m_Level->SelectPrevEntity();
  }

  if (ImGui::InputFloat3("Position", &m_Position.x)) {
    m_Level->SetEntityPosition(m_Position);
  }

  if (ImGui::Button("Save")) {
    m_Level->SaveLevel("Test.json");
  }

  ImGui::End();
}
