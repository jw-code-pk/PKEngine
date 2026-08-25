#include "Spline.h"

Spline::Spline(Ogre::SceneNode *Root) : Curve(Root) { m_CanTick = false; }

bool Spline::Init() {

  m_Length = 1000;
  m_Spline.addPoint(Ogre::Vector3(0, 0, 0));
  m_Spline.addPoint(Ogre::Vector3(m_Length, 0, 0));
  m_Spline.recalcTangents();

  Curve::ShowGizmos();

  return true;
}

Ogre::Vector3 Spline::Evaluate(const float &Distance) {
  auto k = Distance / GetLength();

  return m_Spline.interpolate(k);
}
