#include "Arc.h"
#include <cassert>

Arc::Arc(Ogre::SceneNode *Root) : Curve(Root) { m_CanTick = false; }

bool Arc::Init() { return true; }

float Arc::GetLength() { return m_Angle.valueRadians() * m_Radius; }

Ogre::Vector3 Arc::Evaluate(const float &Distance) {

  // L = angle * r, angle = L / r

  auto a = Distance / m_Radius;
  auto x = m_Radius * Ogre::Math::Cos(a);
  auto z = m_Radius * Ogre::Math::Sin(a);

  return Ogre::Vector3(x, 0, z);
}

void Arc::Setup(const float &Radius, const Ogre::Degree &Angle) {
  assert(Radius > 0 && "Arc radius length must be larger than 0.");

  m_Radius = Radius;
  m_Angle = Angle;

  Curve::ShowGizmos();
}
