#include "EditorGUI.h"
#include "EditorLevel.h"
#include "imgui.h"
#include <OgrePrerequisites.h>

const int PANEL_GAP = 8;

EditorGUI::EditorGUI(EditorLevel *Level) {
  m_Level = Level;
  m_EntityTypeSelectIndex = -1;
}

void EditorGUI::Tick(const float &DeltaTime) {
  ImGui::Begin("Editor");

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("PK Engine")) {
      if (ImGui::MenuItem("Save Level")) {
        m_Level->SaveLevel("Test.json");
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  DrawLeftPanel();
  ImGui::SameLine();
  DrawRightPanel();
  ImGui::End();
}

void EditorGUI::DrawLeftPanel() {
  auto windowX = ImGui::GetContentRegionAvail().x;
  auto panelW = windowX * 0.4f;

  ImGui::BeginChild("LeftPanel", ImVec2(panelW, 0), true);

  // Entity creation
  ImGui::SeparatorText("Create");

  auto entityList = m_Level->GetAvailableEntities();
  auto selectedId = m_EntityTypeSelectIndex < 0
                        ? "None"
                        : entityList[m_EntityTypeSelectIndex];

  if (ImGui::BeginCombo("Entity Type", selectedId.c_str())) {
    for (int i = 0; i < entityList.size(); i++) {
      if (ImGui::Selectable(entityList[i].c_str(),
                            i == m_EntityTypeSelectIndex)) {
        m_EntityTypeSelectIndex = i;
      }
    }
    ImGui::EndCombo();
  }

  if (ImGui::Button("Create")) {
    m_Level->CreateEntity(entityList[m_EntityTypeSelectIndex]);
  }

  // Entity select
  ImGui::SeparatorText("Select");

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

  // Entity transform
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

  // Camera operations
  ImGui::SeparatorText("Camera");

  if (ImGui::Button("Snap Camera")) {
    m_Level->CameraToEntity();
  }

  // Level operations
  ImGui::SeparatorText("Level");

  if (ImGui::Button("Start")) {
    m_Level->BeginPlay();
    m_Level->SetTickEnabled(true);
  }

  ImGui::SameLine();

  if (ImGui::Button("Stop")) {
    m_Level->SetTickEnabled(false);
    m_Level->EndPlay();
  }

  ImGui::EndChild();
}

void EditorGUI::DrawRightPanel() {
  auto windowX = ImGui::GetContentRegionAvail().x;
  auto panelW = windowX - PANEL_GAP;

  ImGui::BeginChild("RightPanel", ImVec2(panelW, 0), true);

  Entity *entity;

  if (m_Level->TryGetEntity(entity)) {
    if (entity->GetTypeId() == "Arc") {
      DrawArcControls(entity);
    } else if (entity->GetTypeId() == "Line") {
      DrawLineControls(entity);
    }
  }

  ImGui::EndChild();
}

void EditorGUI::DrawArcControls(Entity *entity) {
  auto meta = entity->GetMetadata();
  auto angle = meta["angle"].get<float>();
  auto radius = meta["radius"].get<float>();
  auto winding = meta["winding"].get<int>();

  if (ImGui::InputFloat("Angle", &angle)) {
    meta["angle"] = angle;
    entity->SetMetadata(meta);
  }

  if (ImGui::InputFloat("Radius", &radius)) {
    meta["radius"] = radius;

    if (radius > 0) {
      entity->SetMetadata(meta);
    }
  }

  if (ImGui::InputInt("Winding", &winding)) {
    meta["winding"] = winding;

    if (winding == -1 || winding == 1) {
      entity->SetMetadata(meta);
    }
  }
}

void EditorGUI::DrawLineControls(Entity *entity) {
  auto meta = entity->GetMetadata();
  auto len = meta["len"].get<float>();
  auto dir = meta["dir"];
  auto dirVec = Ogre::Vector3(dir["x"], dir["y"], dir["z"]);

  if (ImGui::InputFloat("Length", &len)) {
    meta["len"] = len;
    entity->SetMetadata(meta);
  }

  if (ImGui::InputFloat3("Direction", &dirVec.x)) {
    meta["dir"] = {{"x", dirVec.x}, {"y", dirVec.y}, {"z", dirVec.z}};
    entity->SetMetadata(meta);
  }
}
