#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world) {
  auto pos = world->getCat();
  std::vector<Point2D> path = generatePath(world);
  if (!path.empty()) {
    std::cout <<"CAT: SHMOOVIN\n";
    return path.back();
  }
  std::cout <<"CAT: RANDOM\n";
  auto rand = Random::Range(0, 5);
  switch (rand) {
    case 0:
      return World::NE(pos);
    case 1:
      return World::NW(pos);
    case 2:
      return World::E(pos);
    case 3:
      return World::W(pos);
    case 4:
      return World::SW(pos);
    case 5:
      return World::SE(pos);
    default:
      throw "random out of range";
  }
}
