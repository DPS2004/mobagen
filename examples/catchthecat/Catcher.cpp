#include "Catcher.h"
#include "World.h"
#include <algorithm>
#include <random>


Point2D Catcher::Move(World* world) {
  auto side = world->getWorldSideSize() / 2;
  auto cat = world->getCat();
  std::vector<Point2D> basePath = generatePath(world);
  if (!basePath.empty()) {
    Point2D catGoal = basePath.front();
    if (world->catCanMoveToPosition(catGoal)) {
      //cat is about to win!! stop it!!!
      std::cout <<"CATCHER: CAT IS ABOUT TO WIN. BLOCKING END\n";
      return catGoal;
    }
    if (currentStrategy == HOLE_WALL) {

      int neighbors = NeighborCount(world, catGoal);
      if (neighbors==0) { //no neighbors, just fill it in.

        std::cout <<"CATCHER: END HAS NO NEIGHBORS. BLOCKING END\n";
        return catGoal;
      }

      std::cout <<"CATCHER: GOAL HAS " << neighbors << " NEIGHBORS\n";
      if (neighbors==1) { //end goal is isolated
        std::vector<Point2D> endNeighbors;
        endNeighbors.push_back(World::NE(catGoal));
        endNeighbors.push_back(World::NW(catGoal));
        endNeighbors.push_back(World::E(catGoal));
        endNeighbors.push_back(World::W(catGoal));
        endNeighbors.push_back(World::SE(catGoal));
        endNeighbors.push_back(World::SW(catGoal));
        for (auto n : endNeighbors) {
          if (!world->getContent(n) && world->catcherCanMoveToPosition(n) && world->catWinsOnSpace(n)) {
            std::cout << "CATCHER: FOUND WINNING SPOT NEXT TO GOAL\n";
            return n;
          }
        }
      }
      std::cout << "CATCHER: CHECKING ALT PATH\n";
      std::vector<Point2D> altPath = generatePath(world,catGoal);
      if (altPath.empty()) {
        std::cout <<"CATCHER: ALT PATH HAS NO SOLUTION.\n";
        return catGoal;

      }

      std::cout <<"CATCHER: BLOCKING ALT PATH END.\n";
      std::cout << world->catcherCanMoveToPosition(altPath.front()) << "\n";
      std::cout << world->getContent(altPath.front()) << "\n";
      return altPath.front();
      std::cout <<"CATCHER: BLOCKING NEXT MOVE.\n";
      return basePath.back();

    }

    if (currentStrategy == LONGEST_PATH) {

      std::vector<Point2D> possibleMoves;
      possibleMoves.push_back(basePath.front());
      possibleMoves.push_back(basePath.back());
      possibleMoves.push_back(World::NE(cat));
      possibleMoves.push_back(World::NW(cat));
      possibleMoves.push_back(World::E(cat));
      possibleMoves.push_back(World::W(cat));
      possibleMoves.push_back(World::SE(cat));
      possibleMoves.push_back(World::SW(cat));

      int longestLength = 0;
      Point2D longestMove;
      for (auto m : possibleMoves) {

        std::vector<Point2D> altPath = generatePath(world,m);
        if (altPath.empty()) {
          std::cout <<"CATCHER: ALT PATH HAS NO SOLUTION.\n";
          return m;
        }
        if (altPath.size() > longestLength) {
          longestLength = altPath.size();
          longestMove = m;
        }
      }
      std::cout <<"CATCHER: LONGEST SOLUTION WAS. " << longestLength << "\n";

      return longestMove;

    }


  }
  std::cout <<"CATCHER: PICKING RANDOM ADJACENT.\n";
  std::vector<Point2D> catNeighbors;
  catNeighbors.push_back(World::NE(cat));
  catNeighbors.push_back(World::NW(cat));
  catNeighbors.push_back(World::E(cat));
  catNeighbors.push_back(World::W(cat));
  catNeighbors.push_back(World::SE(cat));
  catNeighbors.push_back(World::SW(cat));
  std::ranges::shuffle(catNeighbors, std::random_device{});
  for (auto n : catNeighbors) {
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
int Catcher::NeighborCount(World* w, Point2D p) {
  int neighbors = 0;
  neighbors += w->getContent(w->NE(p));
  neighbors += w->getContent(w->NW(p));
  neighbors += w->getContent(w->E(p));
  neighbors += w->getContent(w->W(p));
  neighbors += w->getContent(w->SE(p));
  neighbors += w->getContent(w->SW(p));
  return neighbors;
}
