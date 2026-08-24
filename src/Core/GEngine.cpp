#include "GEngine.h"

template <> GEngine *Ogre::Singleton<GEngine>::msSingleton = nullptr;

GEngine::GEngine() {}

GEngine::~GEngine() { m_Resources.clear(); }

GEngine *GEngine::GetInstance() { return msSingleton; }
