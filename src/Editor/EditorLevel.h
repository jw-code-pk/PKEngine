#pragma once

#include "Visuals/OctreeVisualiser.h"

#include "Core/Curves/CurveGroup.h"
#include "Core/Entity.h"
#include "Core/GUI.h"
#include "Core/Level.h"
#include "Core/Triggers/TriggerGroupRegistry.h"

#include <OgrePrerequisites.h>

class EditorLevel : public Level {
public:
  enum class DebugVisualType {
    None,
    CurveOctree,
  };

  virtual void Init() override;
  virtual void Cleanup() override;

  virtual void BeginPlay() override;
  virtual void Tick(const float &DeltaTime) override;
  virtual void EndPlay() override;

  void SelectNextEntity();
  void SelectPrevEntity();

  Ogre::Vector3 GetEntityPosition();
  void SetEntityPosition(const Ogre::Vector3 &Position);

  Ogre::Vector3 GetEntityRotation();
  void SetEntityRotation(const Ogre::Vector3 &Rotation);

  Ogre::String GetEntityTypeId();
  bool TryGetEntity(Entity *&OutEntity);

  void CameraToEntity();
  void SetCameraDistance(const float &Distance);
  void SetCameraDirection(const Ogre::Vector3 &Direction);

  void SaveLevel(const Ogre::String &Name);
  void LoadLevel(const Ogre::String &Name);

  std::vector<Ogre::String> GetAvailableEntities();
  void CreateEntity(const Ogre::String &TypeId,
                    const Ogre::Vector3 &Position = Ogre::Vector3::ZERO);
  void DeleteEntity();

  void ShowDebugVisuals(const DebugVisualType &VisualType);

protected:
  Entity *SpawnFromTypeId(const Ogre::String &TypeId);

  void DisplayTestImage();

private:
  Ogre::SceneNode *m_Camera;
  Ogre::Vector3 m_CameraDirection;
  float m_CameraDistance;

  CurveGroup *m_CurveGroup;
  OctreeVisualiser<Curve *> *m_CurveTreeVisualiser;

  TriggerGroupRegistry *m_TriggerGroupRegistry;

  int m_CurrentIndex;
  GUI *m_GUI;
};
