#include "Line.h"

Line::Line(Ogre::SceneNode *Root) : Curve(Root) { m_CanTick = false; }

void Line::Setup(const Ogre::Vector3 &Direction, const float &Length) {
  m_Direction = Direction;
  m_Length = Length;
}

bool Line::Init() { return true; }

float Line::GetLength() { return m_Length; }

Ogre::Vector3 Line::Evaluate(const float &Distance) {
  return m_Direction * Distance;
}

void Line::SetMetadata(const nlohmann::json &Metadata) {
  if (Metadata == nullptr) {
    return;
  }

  m_Length = Metadata["len"];
  auto dir = Metadata["dir"];

  m_Direction = Ogre::Vector3(dir["x"], dir["y"], dir["z"]);
}

nlohmann::json Line::GetMetadata() const {
  nlohmann::json meta;

  meta["dir"] = {
      {"x", m_Direction.x}, {"y", m_Direction.y}, {"z", m_Direction.z}};

  meta["len"] = m_Length;

  return meta;
}
