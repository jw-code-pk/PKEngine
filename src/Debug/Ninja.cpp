#include "Ninja.h"
#include "Core/Curves/CurveGroup.h"
#include "Core/GEngine.h"
#include "Core/Input/InputState.h"
#include "Core/World.h"
#include <OgreMath.h>
#include <cassert>

Ninja::Ninja(Ogre::SceneNode *Root) : CurveFollower(Root) { m_CanTick = true; }

bool Ninja::Init() {
  auto world = GEngine::Get<World>();
  auto m_SceneManager = world->GetSceneManager();

  Ogre::Entity *ninjaMesh = m_SceneManager->createEntity("ninja.mesh");
  m_PawnNode = GetRoot()->createChildSceneNode();
  m_PawnNode->attachObject(ninjaMesh);
  m_AnimSet = ninjaMesh->getAllAnimationStates();

  GetRoot()->setPosition(0, 0, 0);

  // TODO: Debug - print all anim states

  if (m_AnimSet) {
    auto it = m_AnimSet->getAnimationStateIterator();
    while (it.hasMoreElements()) {
      auto animState = it.getNext();
      GEngine::Log("Animation Name: " + animState->getAnimationName());
    }
  }

  ChangeAnim("Idle2");

  return true;
}

void Ninja::Tick(const float &DeltaTime) {
  assert(m_PawnNode && "Pawn node should be initialised.");

  CurveFollower::Tick(DeltaTime);

  auto inputState = GEngine::Get<InputState>();

  if (inputState->HasAxisInput() && CurveFollower::IsCoyote()) {
    auto curveGroup = GEngine::Get<CurveGroup>();
    FollowClosest(GetRoot()->getPosition(), curveGroup);
  }

  auto speed = inputState->Axis.x * 200;
  CurveFollower::SetSpeed(speed);

  if (inputState->HasAxisInput()) {
    ChangeAnim("Walk");
  } else {
    ChangeAnim("Idle2");
  }

  m_PawnNode->setOrientation(CurveFollower::CalculateOrientation());
  m_ActiveAnim->addTime(DeltaTime);
}

void Ninja::ChangeAnim(const Ogre::String &Name) {
  assert(m_AnimSet->hasAnimationState(Name) &&
         std::format("Animation state {} is not available", Name).c_str());

  if (m_ActiveAnim != nullptr) {
    if (m_ActiveAnim->getAnimationName() == Name) {
      return;
    }

    m_ActiveAnim->setTimePosition(0);
    m_ActiveAnim->setEnabled(false);
    m_ActiveAnim->setLoop(false);
  }

  m_ActiveAnim = m_AnimSet->getAnimationState(Name);
  m_ActiveAnim->setEnabled(true);
  m_ActiveAnim->setLoop(true);
}
