#include <iostream>

#include "Core/App.h"
#include "Core/GEngine.h"

int main(int argc, char *argv[]) {
  try {
    auto gEngine = new GEngine();

    App app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();

    delete gEngine;
  } catch (const std::exception &e) {
    std::cerr << "An exception has occurred: " << e.what() << std::endl;
  }
  return 0;
}
