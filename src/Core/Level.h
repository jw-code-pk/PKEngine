#pragma once

#include "Core/Entity.h"

#include <vector>

class Level {
public:
  virtual ~Level() = default;

  virtual void Init() = 0;
  virtual void Cleanup() = 0;

  virtual void BeginPlay();
  virtual void Tick(const float &DeltaTime);
  virtual void EndPlay();

  void SetTickEnabled(const bool &bIsEnabled) { m_IsTickEnabled = bIsEnabled; }

protected:
  std::vector<Entity *> m_TickList;
  std::vector<Entity *> m_Entities;

  bool m_IsTickEnabled;
};
