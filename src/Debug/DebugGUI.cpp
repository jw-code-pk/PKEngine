#include "DebugGUI.h"
#include "Debug/DebugLevel.h"
#include "imgui.h"
#include <OgrePrerequisites.h>

DebugGUI::DebugGUI(DebugLevel *Level) { m_Level = Level; }

void DebugGUI::Tick(const float &DeltaTime) {
  ImGui::Begin("Debug");

  if (ImGui::Button("Add Cube")) {
  }

  if (ImGui::Button("Save")) {
    m_Level->SaveLevel("Test.json");
  }

  if (ImGui::Button("Prev")) {
    m_Level->SelectPrevEntity();
  }

  ImGui::SameLine();

  if (ImGui::Button("Next")) {
    m_Level->SelectNextEntity();
  }

  auto pos = m_Level->GetEntityPosition();

  if (ImGui::InputFloat3("Position", &pos.x)) {
    m_Level->SetEntityPosition(pos);
  }

  auto rot = m_Level->GetEntityRotation();

  if (ImGui::InputFloat3("Rotation", &rot.x)) {
    m_Level->SetEntityRotation(rot);
  }

  auto entityId = m_Level->GetEntityTypeId();

  if (entityId == "Spline") {
  ImGuiL:
    ImGui::Text("Spline Selected");
  }

  ImGui::End();
}
