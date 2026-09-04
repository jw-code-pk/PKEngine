#pragma once

#include "Entity.h"
#include "GEngine.h"
#include "World.h"

#include <concepts>
#include <vector>

class EntityFactory {
public:
  using EntityCreator = std::function<Entity *()>;

  EntityFactory() { GEngine::Register<EntityFactory>(this); }

  ~EntityFactory() {
    GEngine::Unregister<EntityFactory>(this);
    m_Creators.clear();
    m_AvailableEntities.clear();
  }

  [[nodiscard]] Entity *Spawn(const Ogre::String &TypeId) {
    auto iter = m_Creators.find(TypeId);

    if (iter != m_Creators.end()) {
      auto creator = iter->second;
      return creator();
    }

    return nullptr;
  };

  std::vector<Ogre::String> GetAvailableEntities() {
    return m_AvailableEntities;
  }

  template <std::derived_from<Entity> T>
  void RegisterEntity(const Ogre::String &TypeId) {
    assert(!m_Creators.contains(TypeId) &&
           "Duplicate entity creator being registered.");

    m_Creators[TypeId] = [this]() -> Entity * { return Create<T>(); };
    m_AvailableEntities.push_back(TypeId);
  }

protected:
  template <std::derived_from<Entity> T> [[nodiscard]] T *Create() {
    auto world = GEngine::Get<World>();
    return world->CreateEntity<T>();
  }

private:
  std::vector<Ogre::String> m_AvailableEntities;
  std::unordered_map<Ogre::String, EntityCreator> m_Creators;
};
