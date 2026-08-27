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
  auto sceneManager = world->GetSceneManager();

  GetRoot()->setPosition(0, 0, 0);

  Ogre::Entity *ninjaMesh = sceneManager->createEntity("ninja.mesh");
  m_PawnNode = GetRoot()->createChildSceneNode();
  m_PawnNode->attachObject(ninjaMesh);
  m_AnimSet = ninjaMesh->getAllAnimationStates();
  m_PawnNode->setPosition(Ogre::Vector3::ZERO);

  Ogre::Camera *cam = world->CreateCamera("NinjaCam");
  auto camNode = GetRoot()->createChildSceneNode();
  camNode->attachObject(cam);
  camNode->setPosition(0, 500, 1250);

  auto pitch = -0.15 * Ogre::Math::HALF_PI;

  camNode->pitch(Ogre::Radian(pitch));

  // auto camDir = GetRoot()->getPosition() - camNode->getPosition();
  // camDir.normalise();
  // camNode->setDirection(camDir);

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

void Ninja::BeginPlay() {
  auto world = GEngine::Get<World>();
  world->SetActiveCamera("NinjaCam");
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

  if (speed > 0) {
    m_PawnNode->setDirection(Ogre::Vector3::UNIT_X);
  } else if (speed < 0) {
    m_PawnNode->setDirection(-1 * Ogre::Vector3::UNIT_X);
  }

  GetRoot()->setOrientation(CurveFollower::CalculateOrientation());
  m_ActiveAnim->addTime(DeltaTime * 1.5);
}

void Ninja::EndPlay() {}

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
