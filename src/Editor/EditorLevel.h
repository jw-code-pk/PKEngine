#pragma once

#include "Core/Entity.h"
#include "Core/GUI.h"
#include "Core/Levels/GameplayLevel.h"
#include "Visuals/OctreeVisualiser.h"

#include <OgrePrerequisites.h>

class EditorLevel : public GameplayLevel {
public:
  enum class DebugVisualType {
    None,
    CurveOctree,
  };

  virtual void BeginPlay() override;
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

  void SaveTo(const Ogre::String &Name);

  std::vector<Ogre::String> GetAvailableEntities();
  void CreateEntity(const Ogre::String &TypeId,
                    const Ogre::Vector3 &Position = Ogre::Vector3::ZERO);
  void DeleteEntity();

  void ShowDebugVisuals(const DebugVisualType &VisualType);

protected:
  virtual void CreateResources() override;
  virtual void DestroyResources() override;

  void DisplayTestImage();

private:
  Ogre::SceneNode *m_Camera;
  Ogre::Vector3 m_CameraDirection;
  float m_CameraDistance;

  OctreeVisualiser<Curve *> *m_CurveTreeVisualiser;
  GUI *m_GUI;

  int m_CurrentIndex;
};
