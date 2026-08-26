#include "DebugGUI.h"
#include "Debug/DebugLevel.h"
#include "imgui.h"
#include <OgrePrerequisites.h>

const char *ENTITY_LIST[] = {"Ninja", "Arc", "Spline", "Line", "Cube"};

DebugGUI::DebugGUI(DebugLevel *Level) { m_Level = Level; }

void DebugGUI::Tick(const float &DeltaTime) {
  ImGui::Begin("Debug");

  // Entity creation
  ImGui::SeparatorText("Create");

  ImGui::Combo("Entity Type", &m_EntityTypeSelectIndex, ENTITY_LIST,
               IM_ARRAYSIZE(ENTITY_LIST));

  if (ImGui::Button("Create")) {
    m_Level->CreateEntity(ENTITY_LIST[m_EntityTypeSelectIndex]);
  }

  // Entity focus
  ImGui::SeparatorText("Entity");

  auto entityId = m_Level->GetEntityTypeId();
  ImGui::Text("Selected [%s]", entityId.c_str());

  ImGui::SameLine();

  if (ImGui::Button("Delete")) {
    m_Level->DeleteEntity();
  }

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
  ImGui::SeparatorText("Transform");

  auto pos = m_Level->GetEntityPosition();

  if (ImGui::InputFloat3("Position", &pos.x)) {
    m_Level->SetEntityPosition(pos);
  }

  ImGui::SameLine();

  if (ImGui::Button("PX")) {
    m_Level->SetEntityPosition(Ogre::Vector3::ZERO);
  }

  auto rot = m_Level->GetEntityRotation();

  if (ImGui::InputFloat3("Rotation", &rot.x)) {
    m_Level->SetEntityRotation(rot);
  }

  ImGui::SameLine();

  if (ImGui::Button("RX")) {
    m_Level->SetEntityRotation(Ogre::Vector3::ZERO);
  }

  // Level operations
  ImGui::SeparatorText("Level");

  if (ImGui::Button("Start")) {
    m_Level->SetTickEnabled(true);
  }

  ImGui::SameLine();

  if (ImGui::Button("Stop")) {
    m_Level->SetTickEnabled(false);
  }

  ImGui::Separator();

  if (ImGui::Button("Save Level")) {
    m_Level->SaveLevel("Test.json");
  }

  ImGui::End();
}
