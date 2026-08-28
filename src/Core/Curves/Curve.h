#pragma once

#include "Core/Entity.h"

class Curve : public Entity {
public:
  static const int NoneID = -1;

  struct QueryResult {
    float Distance;
    Ogre::Vector3 Point;
  };

  Curve(Ogre::SceneNode *Root);

  virtual float GetLength() = 0;
  virtual Ogre::Vector3 Evaluate(const float &Distance) = 0;

  void ShowGizmos();

  int GetCurveId() const { return m_CurveID; }

  QueryResult FindClosestPoint(const Ogre::Vector3 &Position,
                               const float &Step = 1.0f);

  QueryResult FindClosestPointIgnoreY(const Ogre::Vector3 &Position,
                                      const float &Step = 1.0f);

private:
  int m_CurveID;
};
