#include <iostream>

#include "Core/App.h"
#include "Core/GEngine.h"

#include "Debug/DebugEntityFactory.h"

int main(int argc, char *argv[]) {
  try {
    auto gEngine = new GEngine();
    auto entityFactory = new DebugEntityFactory();

    App app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();

    delete entityFactory;
    delete gEngine;
  } catch (const std::exception &e) {
    std::cerr << "An exception has occurred: " << e.what() << std::endl;
  }
  return 0;
}
