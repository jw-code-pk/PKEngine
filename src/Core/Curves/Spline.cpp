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

  auto localPos = m_Spline.interpolate(k);
  return GetRoot()->convertLocalToWorldPosition(localPos);
}

void Spline::SetMetadata(const nlohmann::json &Metadata) {
  if (Metadata == nullptr) {
    return;
  }

  for (const auto &p : Metadata["points"]) {
    auto point = Ogre::Vector3(p["x"], p["y"], p["z"]);
  }
}

nlohmann::json Spline::GetMetadata() const {
  nlohmann::json meta;

  meta["points"] = nlohmann::json::array();

  for (int i = 0; i < m_Spline.getNumPoints(); i++) {
    auto p = m_Spline.getPoint(i);
    meta["points"].push_back({{"x", p.x}, {"y", p.y}, {"z", p.z}});
  }

  return meta;
}
