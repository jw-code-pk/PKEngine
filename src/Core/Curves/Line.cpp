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
