#pragma once

#include "Curve.h"

class Arc : public Curve {
public:
  Arc(Ogre::SceneNode *Root);

  virtual Ogre::String GetTypeId() override { return "Arc"; }
  virtual bool Init() override;
  virtual float GetLength() override;
  virtual Ogre::Vector3 Evaluate(const float &Distance) override;
  virtual void SetMetadata(const nlohmann::json &Metadata) override;
  virtual nlohmann::json GetMetadata() const override;

  void Setup(const float &Radius, const Ogre::Degree &Angle,
             const float &Winding = 1);

private:
  float m_Radius;
  int m_Winding;
  Ogre::Degree m_Angle;
};
