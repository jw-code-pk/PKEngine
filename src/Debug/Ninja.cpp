#include "Ninja.h"
#include "NinjaSM/States/NinjaIdleState.h"
#include "NinjaSM/States/NinjaJumpState.h"
#include "NinjaSM/States/NinjaWalkState.h"

#include "Triggers/BounceTrigger.h"

#include "Core/GEngine.h"
#include "Core/Triggers/TriggerGroupRegistry.h"
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

  if (m_TriggerSweeper != nullptr) {
    delete m_TriggerSweeper;
  }
}

bool Ninja::Init() {
  auto world = GEngine::Get<World>();
  auto sceneManager = world->GetSceneManager();

  GetRoot()->setPosition(0, 0, 0);

  m_PawnNode = GetRoot()->createChildSceneNode();

  // model setup
  Ogre::Entity *ninjaMesh = sceneManager->createEntity("ninja.mesh");
  m_ModelNode = m_PawnNode->createChildSceneNode();
  m_ModelNode->attachObject(ninjaMesh);
  m_AnimSet = ninjaMesh->getAllAnimationStates();
  m_ModelNode->setPosition(Ogre::Vector3::ZERO);

  // camera setup
  Ogre::Camera *cam = world->CreateCamera("NinjaCam");
  auto camNode = m_PawnNode->createChildSceneNode();
  camNode->attachObject(cam);
  camNode->setPosition(0, 500, 1250);

  auto pitch = -0.15 * Ogre::Math::HALF_PI;
  camNode->pitch(Ogre::Radian(pitch));

  // Trigger Sweep
  if (m_TriggerSweeper == nullptr) {
    auto groupRegistry = GEngine::Get<TriggerGroupRegistry>();
    auto group = groupRegistry->Get("Player");
    m_TriggerSweeper = new TriggerSweeper(this, group);
  }

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

  auto curveGroup = GEngine::Get<CurveGroup>();
  FollowClosest(GetRoot()->getPosition(), curveGroup);

  m_StateMachine->ChangeState<NinjaIdleState>();
}

void Ninja::Tick(const float &DeltaTime) {
  assert(m_PawnNode && "Pawn node should be initialised.");
  m_StateMachine->Tick(DeltaTime);

  TickPhysics(DeltaTime);

  m_TriggerSweeper->RunSweep();

  GetRoot()->setOrientation(CurveFollower::CalculateOrientation());
  m_ActiveAnim->addTime(DeltaTime * 3);
}

void Ninja::EndPlay() {}

Ogre::AxisAlignedBox Ninja::GetAAB() {
  const auto origin = GetRoot()->getPosition() + m_PawnNode->getPosition();
  const auto extents = Ogre::Vector3::UNIT_SCALE * 50;
  return Ogre::AxisAlignedBox(origin - extents, origin + extents);
}

bool Ninja::OnEnter(Trigger *Tripped) {
  GEngine::Log("Ninja tripped a trigger.");

  if (auto bounce = dynamic_cast<BounceTrigger *>(Tripped)) {
    GEngine::Log("Ninja bounce hit.");
    m_StateMachine->ChangeState<NinjaJumpState>();
  }
  return true;
}

bool Ninja::OnExit(Trigger *Tripped) { return true; }

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
  if (m_IsGrounded) {
    TickGroundMovement(DeltaTime);
  }

  if (!m_IsGrounded) {
    TickInAirMovement(DeltaTime);
  }

  CurveFollower::Tick(DeltaTime);
}

void Ninja::TickGroundMovement(const float &DeltaTime) {
  auto curveGroup = GEngine::Get<CurveGroup>();

  // Should we change to a connected curve?
  if (IsCoyote()) {
    FollowClosest(GetRoot()->getPosition(), curveGroup, true);
  }

  // There is no connected curve - should we need to fall
  if (IsCoyote()) {
    m_IsGrounded = false;
  }
}

void Ninja::TickInAirMovement(const float &DeltaTime) {
  auto curveGroup = GEngine::Get<CurveGroup>();

  const auto prevFollowPos = GetRoot()->getPosition();
  const auto worldPosition = prevFollowPos + m_PawnNode->getPosition();
  FollowClosestBelow(worldPosition, curveGroup, IsCoyote());
  const auto nextFollowPos = GetRoot()->getPosition();
  m_GroundDistance += prevFollowPos.y - nextFollowPos.y;

  const auto gravity = 7000;
  m_GroundDistance += m_VerticalSpeed * DeltaTime;

  if (m_GroundDistance <= 0) {
    // Land
    m_IsGrounded = true;
    m_VerticalSpeed = 0;
    m_GroundDistance = 0;
    m_PawnNode->setPosition(Ogre::Vector3::ZERO);
  } else {
    // Update vertical movement
    m_VerticalSpeed -= gravity * DeltaTime;
    auto updatedPos = m_PawnNode->getPosition();
    updatedPos.y = m_GroundDistance;
    m_PawnNode->setPosition(updatedPos);
  }
}
