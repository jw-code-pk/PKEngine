#pragma once

#include "Core/Curves/CurveGroup.h"
#include "Core/Entity.h"
#include "Core/GUI.h"
#include "Core/Level.h"
#include <OgrePrerequisites.h>

class DebugLevel : public Level {
public:
  virtual void Init() override;
  virtual void Cleanup() override;
  virtual void Tick(const float &DeltaTime) override;

  void SelectNextEntity();
  void SelectPrevEntity();

  Ogre::Vector3 GetEntityPosition();
  void SetEntityPosition(const Ogre::Vector3 &Position);

  Ogre::Vector3 GetEntityRotation();
  void SetEntityRotation(const Ogre::Vector3 &Rotation);

  Ogre::String GetEntityTypeId();
  bool TryGetEntity(Entity *&OutEntity);

  void CameraToEntity();

  void SaveLevel(const Ogre::String &Name);
  void LoadLevel(const Ogre::String &Name);

  void CreateEntity(const Ogre::String &TypeId,
                    const Ogre::Vector3 &Position = Ogre::Vector3::ZERO);
  void DeleteEntity();

protected:
  Entity *SpawnFromTypeId(const Ogre::String &TypeId);

  void DisplayTestImage();

private:
  Ogre::SceneNode *m_Camera;
  CurveGroup *m_CurveGroup;

  int m_CurrentIndex;
  GUI *m_GUI;
};
