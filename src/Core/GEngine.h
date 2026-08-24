#pragma once

#include <cassert>
#include <typeindex>
#include <unordered_map>

#include <OgreSingleton.h>

class GEngine : public Ogre::Singleton<GEngine> {
public:
  GEngine();
  ~GEngine();

  static GEngine *GetInstance();

  template <typename T> static void Register(T *Res) {
    assert(Res && "Null resources instance cannot be registered.");
    assert(msSingleton && "GEngine instance not ready yet.");

    const auto key = std::type_index(typeid(T));
    const auto val = static_cast<void *>(Res);

    msSingleton->m_Resources[key] = val;
  }

  template <typename T> static bool TryGet(T *&Res) {
    assert(msSingleton && "GEngine instance not ready yet.");

    Res = nullptr;

    const auto key = std::type_index(typeid(T));
    auto iter = msSingleton->m_Resources.find(key);
    if (iter != msSingleton->m_Resources.end()) {
      Res = static_cast<T *>(iter->second);
      return true;
    }

    assert(Res && "Resource was not found.");

    return false;
  }

  template <typename T> static void Unregister(T *Res) {
    assert(msSingleton && "GEngine instance not ready yet.");

    const auto key = std::type_index(typeid(Res));
    msSingleton->m_Resources.erase(key);
  }

private:
  std::unordered_map<std::type_index, void *> m_Resources;
};
