#pragma once

#include "Level.h"

#include "Core/Curves/CurveGroup.h"
#include "Core/Entity.h"
#include "Core/Triggers/TriggerGroupRegistry.h"

#include <OgrePrerequisites.h>

class GameplayLevel : public Level {
public:
  virtual void Load(const Ogre::String &Filename) override;
  virtual void Unload() override;

  virtual void BeginPlay() override;
  virtual void Tick(const float &DeltaTime) override;
  virtual void EndPlay() override;

protected:
  virtual void CreateResources();
  virtual void DestroyResources();
  Entity *SpawnFromTypeId(const Ogre::String &TypeId);

protected:
  CurveGroup *m_CurveGroup;
  TriggerGroupRegistry *m_TriggerGroupRegistry;
};
