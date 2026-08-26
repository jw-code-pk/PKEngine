#include "Line.h"

Line::Line(Ogre::SceneNode *Root) : Curve(Root) { m_CanTick = false; }

bool Line::Init() {
  Setup(Ogre::Vector3::UNIT_X, 1000);
  return true;
}

void Line::Setup(const Ogre::Vector3 &Direction, const float &Length) {
  m_Direction = Direction;
  m_Length = Length;

  Curve::ShowGizmos();
}

float Line::GetLength() { return m_Length; }

Ogre::Vector3 Line::Evaluate(const float &Distance) {
  auto localPos = m_Direction * Distance;
  return GetRoot()->convertLocalToWorldPosition(localPos);
}

void Line::SetMetadata(const nlohmann::json &Metadata) {
  if (Metadata == nullptr) {
    return;
  }

  auto len = Metadata["len"];
  auto dir = Metadata["dir"];

  Setup(Ogre::Vector3(dir["x"], dir["y"], dir["z"]), len);
}

nlohmann::json Line::GetMetadata() const {
  nlohmann::json meta;

  meta["dir"] = {
      {"x", m_Direction.x}, {"y", m_Direction.y}, {"z", m_Direction.z}};

  meta["len"] = m_Length;

  return meta;
}
