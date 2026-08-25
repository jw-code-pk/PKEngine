#pragma once

#include "Core/GUI.h"
#include "Debug/DebugLevel.h"
#include <OgrePrerequisites.h>

class DebugGUI : public GUI {
public:
  DebugGUI(DebugLevel *Level);
  virtual void Tick(const float &DeltaTime) override;

private:
  DebugLevel *m_Level;
  int m_EntityTypeSelectIndex;
};
