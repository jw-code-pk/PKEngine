#include "ResourceLoader.h"
#include "GEngine.h"

#include <OgreConfigFile.h>
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
#include <OgreResourceGroupManager.h>
#include <OgreTechnique.h>

ResourceLoader::ResourceLoader() { GEngine::Register<ResourceLoader>(this); }

ResourceLoader::~ResourceLoader() { GEngine::Unregister<ResourceLoader>(this); }

void ResourceLoader::LoadConfig() {
  Ogre::ConfigFile config;
  config.load("./Assets/resources.cfg");

  auto settings = config.getSettingsBySection();
  auto resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();

  for (auto iter = settings.begin(); iter != settings.end(); ++iter) {
    auto groupName = iter->first;
    auto groupMap = iter->second;

    for (auto item : groupMap) {
      auto itemType = item.first;
      auto archiveName = item.second;

      resourceGroupManager->addResourceLocation(archiveName, itemType,
                                                groupName);
    }
  }

  resourceGroupManager->initialiseAllResourceGroups();
}

void ResourceLoader::CreateUIMaterial(const std::string &GroupName,
                                      const std::string &MaterialName,
                                      const std::string &TextureName) {
  auto mat =
      Ogre::MaterialManager::getSingleton().create(MaterialName, GroupName);
  auto pass = mat->getTechnique(0)->getPass(0);

  pass->setLightingEnabled(false);
  pass->setDepthCheckEnabled(false);
  pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
  pass->createTextureUnitState(TextureName);
}
