#include "DebugGUI.h"
#include "Debug/DebugLevel.h"
#include "imgui.h"
#include <OgrePrerequisites.h>

DebugGUI::DebugGUI(DebugLevel *Level) { m_Level = Level; }

void DebugGUI::Tick(const float &DeltaTime) {
  ImGui::Begin("Debug");

  auto entityId = m_Level->GetEntityTypeId();
  ImGui::Text("%s", entityId.c_str());

  // Entity focus

  if (ImGui::Button("Prev")) {
    m_Level->SelectPrevEntity();
  }

  ImGui::SameLine();

  if (ImGui::Button("Next")) {
    m_Level->SelectNextEntity();
  }

  ImGui::SameLine();

  if (ImGui::Button("Snap Camera")) {
    m_Level->CameraToEntity();
  }

  // Entity placement

  auto pos = m_Level->GetEntityPosition();

  if (ImGui::InputFloat3("Position", &pos.x)) {
    m_Level->SetEntityPosition(pos);
  }

  auto rot = m_Level->GetEntityRotation();

  if (ImGui::InputFloat3("Rotation", &rot.x)) {
    m_Level->SetEntityRotation(rot);
  }

  // Level operations

  if (ImGui::Button("Save Level")) {
    m_Level->SaveLevel("Test.json");
  }

  ImGui::End();
}
