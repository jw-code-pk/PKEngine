#pragma once

#include "Curve.h"
#include <OgreSimpleSpline.h>

class Spline : public Curve {
public:
  Spline(Ogre::SceneNode *Root);

  virtual Ogre::String GetTypeId() override { return "Spline"; }
  virtual bool Init() override;
  virtual float GetLength() override { return m_Length; }
  virtual Ogre::Vector3 Evaluate(const float &Distance) override;
  virtual void SetMetadata(const nlohmann::json &Metadata) override;
  virtual nlohmann::json GetMetadata() const override;

private:
  Ogre::ManualObject *m_Visual;
  Ogre::SimpleSpline m_Spline;
  float m_Length;
};
