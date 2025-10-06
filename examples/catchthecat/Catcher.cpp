#include "Catcher.h"
#include "World.h"
#include <algorithm>
#include <random>

Point2D Catcher::Move(World* world) {
  auto side = world->getWorldSideSize() / 2;
  std::vector<Point2D> path = generatePath(world);
  if (!path.empty()) {
    return path.front();
  }
  std::cout <<"CATCHER has trapped cat, now trying to end game.\n";
  auto cat = world->getCat();
  std::vector<Point2D> neighbors;
  neighbors.push_back(World::NE(cat));
  neighbors.push_back(World::NW(cat));
  neighbors.push_back(World::E(cat));
  neighbors.push_back(World::W(cat));
  neighbors.push_back(World::SE(cat));
  neighbors.push_back(World::SW(cat));
  std::ranges::shuffle(neighbors, std::random_device{});
  for (auto n : neighbors) {
    if (!world->getContent(n)) {
      return n;
    }
  }

  std::cout <<"CATCHER RANDOM\n";
  for (;;) {
    Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
    if (cat.x != p.x && cat.y != p.y && !world->getContent(p)) return p;
  }
}
