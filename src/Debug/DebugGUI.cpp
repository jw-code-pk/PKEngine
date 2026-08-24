#include "DebugGUI.h"
#include "Debug/DebugLevel.h"
#include "imgui.h"
#include <OgrePrerequisites.h>

DebugGUI::DebugGUI(DebugLevel *Level) { m_Level = Level; }

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

  auto pos = m_Level->GetEntityPosition();

  if (ImGui::InputFloat3("Position", &pos.x)) {
    m_Level->SetEntityPosition(pos);
  }

  auto rot = m_Level->GetEntityRotation();

  if (ImGui::InputFloat3("Rotation", &rot.x)) {
    m_Level->SetEntityRotation(rot);
  }

  if (ImGui::Button("Save")) {
    m_Level->SaveLevel("Test.json");
  }

  ImGui::End();
}
