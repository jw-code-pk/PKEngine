#pragma once

#include "Core/GUI.h"
#include "EditorLevel.h"
#include <OgrePrerequisites.h>

class EditorGUI : public GUI {
public:
  EditorGUI(EditorLevel *Level);
  virtual void Tick(const float &DeltaTime) override;

protected:
  void DrawLeftPanel();
  void DrawRightPanel();

  void DrawArcControls(Entity *entity);
  void DrawLineControls(Entity *entity);
  void DrawSplineControls(Entity *entity);

private:
  EditorLevel *m_Level;
  int m_EntityTypeSelectIndex;
};
