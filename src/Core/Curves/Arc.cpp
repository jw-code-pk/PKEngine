#include "Arc.h"
#include <cassert>

Arc::Arc(Ogre::SceneNode *Root) : Curve(Root) { m_CanTick = false; }

bool Arc::Init() {
  Setup(500, Ogre::Degree(90));
  return true;
}

float Arc::GetLength() { return m_Angle.valueRadians() * m_Radius; }

Ogre::Vector3 Arc::Evaluate(const float &Distance) {

  // L = angle * r, angle = L / r

  auto a = (GetLength() - Distance) / m_Radius;
  auto x = m_Radius * Ogre::Math::Cos(a);
  auto z = m_Radius * Ogre::Math::Sin(a) - m_Radius;

  auto localPos = Ogre::Vector3(x, 0, z * m_Winding);

  return GetRoot()->convertLocalToWorldPosition(localPos);
}

void Arc::SetMetadata(const nlohmann::json &Metadata) {
  if (Metadata == nullptr) {
    return;
  }

  auto angle = Metadata["angle"];
  auto radius = Metadata["radius"];
  auto winding =
      Metadata.contains("winding") ? Metadata["winding"].get<int>() : 1;

  Setup(radius, Ogre::Degree(angle), winding);
}

nlohmann::json Arc::GetMetadata() const {
  nlohmann::json meta;

  meta["angle"] = m_Angle.valueDegrees();
  meta["radius"] = m_Radius;
  meta["winding"] = m_Winding;

  return meta;
}

void Arc::Setup(const float &Radius, const Ogre::Degree &Angle,
                const float &Winding) {
  assert(Radius > 0 && "Arc radius length must be larger than 0.");
  assert((Winding == -1 || Winding == 1) && "Winding must be either -1 or 1.");

  m_Radius = Radius;
  m_Winding = Winding;
  m_Angle = Angle;

  Curve::ShowGizmos();
}
