#pragma once

#include "Core/Entity.h"
#include "Core/Level.h"
#include <vector>

class DebugLevel : public Level {
public:
  virtual void Init() override;

  virtual void Tick(const float &DeltaTime) override;

protected:
  void DisplayTestImage();

private:
  std::vector<Entity *> m_TickList;
};
