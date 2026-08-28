#include "Ninja.h"
#include "NinjaSM/States/NinjaIdleState.h"
#include "NinjaSM/States/NinjaJumpState.h"
#include "NinjaSM/States/NinjaWalkState.h"

#include "Core/GEngine.h"
#include "Core/World.h"
#include <OgreMath.h>
#include <cassert>

Ninja::Ninja(Ogre::SceneNode *Root) : CurveFollower(Root) {
  m_CanTick = true;
  m_IsGrounded = true;
  m_StateMachine = new NinjaSM(this);
}

Ninja::~Ninja() {
  m_StateMachine->RemoveAllStates(true);
  delete m_StateMachine;
}

bool Ninja::Init() {
  auto world = GEngine::Get<World>();
  auto sceneManager = world->GetSceneManager();

  GetRoot()->setPosition(0, 0, 0);

  m_PawnNode = GetRoot()->createChildSceneNode();

  Ogre::Entity *ninjaMesh = sceneManager->createEntity("ninja.mesh");
  m_ModelNode = m_PawnNode->createChildSceneNode();
  m_ModelNode->attachObject(ninjaMesh);
  m_AnimSet = ninjaMesh->getAllAnimationStates();
  m_ModelNode->setPosition(Ogre::Vector3::ZERO);

  Ogre::Camera *cam = world->CreateCamera("NinjaCam");
  auto camNode = m_PawnNode->createChildSceneNode();
  camNode->attachObject(cam);
  camNode->setPosition(0, 500, 1250);

  auto pitch = -0.15 * Ogre::Math::HALF_PI;
  camNode->pitch(Ogre::Radian(pitch));

  // TODO: Debug - print all anim states

  if (m_AnimSet) {
    auto it = m_AnimSet->getAnimationStateIterator();
    while (it.hasMoreElements()) {
      auto animState = it.getNext();
      GEngine::Log("Animation Name: " + animState->getAnimationName());
    }
  }

  // Setup behaviour states
  m_StateMachine->Register(new NinjaWalkState());
  m_StateMachine->Register(new NinjaIdleState());
  m_StateMachine->Register(new NinjaJumpState());

  return true;
}

void Ninja::BeginPlay() {
  auto world = GEngine::Get<World>();
  world->SetActiveCamera("NinjaCam");

  m_StateMachine->ChangeState<NinjaIdleState>();
}

void Ninja::Tick(const float &DeltaTime) {
  assert(m_PawnNode && "Pawn node should be initialised.");
  m_StateMachine->Tick(DeltaTime);

  TickPhysics(DeltaTime);

  GetRoot()->setOrientation(CurveFollower::CalculateOrientation());
  m_ActiveAnim->addTime(DeltaTime * 3);
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

void Ninja::FaceDirection(const Ogre::Vector3 &Direction) {
  m_ModelNode->setDirection(Direction);
}

void Ninja::Launch(const float &Speed) {
  if (!m_IsGrounded) {
    return;
  }

  m_VerticalSpeed = Speed;
  m_IsGrounded = false;
}

void Ninja::TickPhysics(const float &DeltaTime) {
  CurveFollower::Tick(DeltaTime);

  auto curveGroup = GEngine::Get<CurveGroup>();

  if (m_IsGrounded) {
    if (IsCoyote()) {
      FollowClosest(GetRoot()->getPosition(), curveGroup, true);

      if (IsCoyote()) {
        m_IsGrounded = false;
      }
    }
  }

  if (!m_IsGrounded) {
    const auto worldPosition =
        GetRoot()->getPosition() + m_PawnNode->getPosition();
    const auto prevFollowPos = GetRoot()->getPosition();
    FollowClosestBelow(worldPosition, curveGroup, false);
    const auto nextFollowPos = GetRoot()->getPosition();
    m_GroundDistance += nextFollowPos.y - prevFollowPos.y;

    const auto gravity = 900;
    m_GroundDistance += m_VerticalSpeed * DeltaTime;

    if (m_GroundDistance <= 0) {
      m_IsGrounded = true;
      m_VerticalSpeed = 0;
      m_GroundDistance = 0;
      m_PawnNode->setPosition(Ogre::Vector3::ZERO);
    } else {
      m_VerticalSpeed -= gravity * DeltaTime;
      auto updatedPos = m_PawnNode->getPosition();
      updatedPos.y = m_GroundDistance;
      m_PawnNode->setPosition(updatedPos);
    }
  }
}
