#pragma once

class Level {
public:
  virtual void Init() = 0;
  virtual void Tick(const float &DeltaTime) = 0;
  virtual void Cleanup() = 0;
};
