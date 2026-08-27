#include "Curve.h"
#include "Core/GEngine.h"
#include "Core/World.h"

#include <cassert>

static int NextID = 0;

Curve::Curve(Ogre::SceneNode *Root) : Entity(Root) { m_CurveID = NextID++; }

void Curve::ShowGizmos() {
  World *world = GEngine::Get<World>();

  auto gizmoId = std::format("{}_{}", GetTypeId(), GetCurveId());
  auto sceneManager = world->GetSceneManager();

  Ogre::ManualObject *visual = nullptr;

  if (sceneManager->hasManualObject(gizmoId)) {
    visual = sceneManager->getManualObject(gizmoId);
    visual->clear();
  } else {
    visual = sceneManager->createManualObject(gizmoId);
    GetRoot()->attachObject(visual);
  }

  visual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

  auto length = GetLength();
  auto numSegments = 10;
  for (int i = 0; i <= numSegments; i++) {
    auto k = static_cast<float>(i) / static_cast<float>(numSegments);
    auto pos = Evaluate(k * length);
    auto localPos = GetRoot()->convertWorldToLocalPosition(pos);

    visual->position(localPos);
    visual->colour(Ogre::ColourValue::Green);
  }

  visual->end();
}

Curve::QueryResult Curve::FindClosestPoint(const Ogre::Vector3 &Position,
                                           const float &Step) {
  assert(Step >= 1.0f && "Step size can't be smaller than 1.0f.");

  const auto length = GetLength();
  const auto numSamples = static_cast<int>(length / Step);

  auto closestDistance = 0.0f;
  auto closestPoint = Evaluate(closestDistance);

  for (int i = 1; i < numSamples; i++) {
    const auto k = i * Step;
    const auto p = Evaluate(k);

    const auto d1 = Position.squaredDistance(closestPoint);
    const auto d2 = Position.squaredDistance(p);

    if (d2 < d1) {
      closestPoint = p;
      closestDistance = k;
    }
  }

  return Curve::QueryResult{
      .Distance = closestDistance,
      .Point = closestPoint,
  };
}
