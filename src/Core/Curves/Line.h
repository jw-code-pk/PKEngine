#pragma once

#include "Curve.h"

class Line : public Curve {
public:
  virtual Ogre::String GetTypeId() override { return "Line"; }

  Line(Ogre::SceneNode *Root);
  void Setup(const Ogre::Vector3 &Direction, const float &Length);

  virtual bool Init() override;
  virtual float GetLength() override;
  virtual Ogre::Vector3 Evaluate(const float &Distance) override;
  virtual void SetMetadata(const nlohmann::json &Metadata) override;
  virtual nlohmann::json GetMetadata() const override;

private:
  Ogre::Vector3 m_Direction;
  float m_Length;
};
