#pragma once

#include <cassert>
#include <concepts>
#include <format>
#include <typeindex>
#include <unordered_map>

#define StateIndex(x) (std::type_index(typeid(x)))

template <typename T> class StateMachine {
public:
  using StatesIterator = std::unordered_map<std::type_index, void *>::iterator;

  class State {
  public:
    virtual ~State() = default;

    virtual void Enter(T *Target) = 0;
    virtual void Tick(T *Target, const float &DeltaTime) = 0;
    virtual void Exit(T *Target) = 0;
  };

  StateMachine(T *Target) {
    m_Target = Target;
    m_CurrentState = nullptr;
  }

  ~StateMachine() { m_States.clear(); };

  void Tick(const float &DeltaTime) {
    if (m_CurrentState != nullptr) {
      m_CurrentState->Tick(m_Target, DeltaTime);
    }
  }

  void RemoveAllStates(const bool &bDelete = false) {
    if (bDelete) {
      for (auto iter : m_States) {
        auto val = iter.second;
        auto state = static_cast<State *>(val);

        delete state;
      }
    }

    m_CurrentState = nullptr;
    m_States.clear();
  }

  template <std::derived_from<State> U> void Register(U *TState) {
    assert(TState && "Null resources instance cannot be registered.");

    auto key = StateIndex(U);
    auto val = TState;

    m_States[key] = val;
  };

  template <std::derived_from<State> U> void ChangeState() {
    auto key = StateIndex(U);
    auto nextState = m_States[key];

    assert(nextState != nullptr && "The requested state is not registered.");

    if (nextState == m_CurrentState) {
      return;
    }

    if (m_CurrentState != nullptr) {
      m_CurrentState->Exit(m_Target);
    }

    m_CurrentState = m_States[key];
    m_CurrentState->Enter(m_Target);
  };

private:
  T *m_Target;
  State *m_CurrentState;
  std::unordered_map<std::type_index, State *> m_States;
};
