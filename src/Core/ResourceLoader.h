#pragma once

#include <string>
class ResourceLoader {
public:
  ResourceLoader();
  ~ResourceLoader();

  void LoadConfig();

  void CreateUIMaterial(const std::string &GroupName,
                        const std::string &MaterialName,
                        const std::string &TextureName);
};
