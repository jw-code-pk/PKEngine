#include "DebugGUI.h"
#include "Debug/DebugLevel.h"
#include "imgui.h"
#include <OgrePrerequisites.h>

const char *ENTITY_LIST[] = {"Ninja", "Arc", "Spline", "Line", "Cube"};

DebugGUI::DebugGUI(DebugLevel *Level) { m_Level = Level; }

void DebugGUI::Tick(const float &DeltaTime) {
  ImGui::Begin("Debug");

  // Entity creation
  ImGui::Combo("Entity Type", &m_EntityTypeSelectIndex, ENTITY_LIST,
               IM_ARRAYSIZE(ENTITY_LIST));

  if (ImGui::Button("Create")) {
    m_Level->CreateEntity(ENTITY_LIST[m_EntityTypeSelectIndex]);
  }

  // Entity focus

  auto entityId = m_Level->GetEntityTypeId();
  ImGui::Text("Selected [%s]", entityId.c_str());

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
