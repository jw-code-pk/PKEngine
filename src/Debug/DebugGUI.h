#pragma once

#include "Core/GUI.h"
#include "Debug/DebugLevel.h"
#include <OgrePrerequisites.h>

class DebugGUI : public GUI {
public:
  DebugGUI(DebugLevel *Level);
  virtual void Tick(const float &DeltaTime) override;

protected:
  void DrawLeftPanel();
  void DrawRightPanel();

  void DrawArcControls(Entity *entity);
  void DrawLineControls(Entity *entity);
  void DrawSplineControls(Entity *entity);

private:
  DebugLevel *m_Level;
  int m_EntityTypeSelectIndex;
};
