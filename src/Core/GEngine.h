#pragma once

#include <cassert>
#include <typeindex>
#include <unordered_map>

#include <Ogre.h>
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

  template <typename T> static T *Get() {
    assert(msSingleton && "GEngine instance not ready yet.");

    const auto key = std::type_index(typeid(T));
    auto iter = msSingleton->m_Resources.find(key);
    if (iter != msSingleton->m_Resources.end()) {
      return static_cast<T *>(iter->second);
    }

    assert(false && "Resource not available.");

    return nullptr;
  }

  template <typename T> static void Unregister(T *Res) {
    assert(msSingleton && "GEngine instance not ready yet.");

    const auto key = std::type_index(typeid(Res));
    msSingleton->m_Resources.erase(key);
  }

  static void Log(const Ogre::String &Msg) {
    Ogre::LogManager::getSingleton().logMessage(Msg);
  }

private:
  std::unordered_map<std::type_index, void *> m_Resources;
};
