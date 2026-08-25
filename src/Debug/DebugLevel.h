#pragma once

#include "Core/Entity.h"
#include "Core/GUI.h"
#include "Core/Level.h"
#include <OgrePrerequisites.h>
#include <vector>

class DebugLevel : public Level {
public:
  virtual void Init() override;
  virtual void Tick(const float &DeltaTime) override;

  void SelectNextEntity();
  void SelectPrevEntity();

  Ogre::Vector3 GetEntityPosition();
  void SetEntityPosition(const Ogre::Vector3 &Position);

  Ogre::Vector3 GetEntityRotation();
  void SetEntityRotation(const Ogre::Vector3 &Rotation);

  Ogre::String GetEntityTypeId();

  void SaveLevel(const Ogre::String &Name);

protected:
  void DisplayTestImage();

private:
  std::vector<Entity *> m_TickList;
  std::vector<Entity *> m_Entities;

  int m_CurrentIndex;

  GUI *m_GUI;
};
