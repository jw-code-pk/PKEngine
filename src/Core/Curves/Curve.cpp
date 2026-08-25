#include "Curve.h"
#include "Core/GEngine.h"
#include "Core/World.h"

void Curve::ShowGizmos() {
  World *world = nullptr;
  const auto bHasWorld = GEngine::TryGet(world);
  assert(bHasWorld && "No world is registered.");

  auto sceneManager = world->GetSceneManager();
  auto visual = sceneManager->createManualObject();

  visual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

  auto length = GetLength();
  auto numSegments = 10;
  for (int i = 0; i < numSegments; i++) {
    auto k = static_cast<float>(i) / static_cast<float>(numSegments);
    auto pos = Evaluate(k * length);

    visual->position(pos);
    visual->colour(Ogre::ColourValue::Green);
  }

  visual->end();

  GetRoot()->attachObject(visual);
  GetRoot()->setPosition(Ogre::Vector3::ZERO);
}
