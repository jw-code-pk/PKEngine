#include "RegisterEntities.h"

#include "Core/Curves/Arc.h"
#include "Core/Curves/Line.h"
#include "Debug/Cube.h"
#include "Debug/Ninja.h"
#include "Debug/Triggers/BounceTrigger.h"

void RegisterEntities(EntityFactory *Factory) {
  Factory->RegisterEntity<Arc>("Arc");
  Factory->RegisterEntity<Line>("Line");
  Factory->RegisterEntity<Ninja>("Ninja");
  Factory->RegisterEntity<Cube>("Cube");
  Factory->RegisterEntity<BounceTrigger>("Bounce");
}
