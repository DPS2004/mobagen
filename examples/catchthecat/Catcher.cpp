#include "Catcher.h"
#include "World.h"
#include <algorithm>
#include <random>


Point2D Catcher::Move(World* world) {

  strategy moveStrategy = currentStrategy;


  auto side = world->getWorldSideSize() / 2;
  auto cat = world->getCat();

  bool prioritizeNeighbors = false;
  std::vector<Point2D> basePath = generatePath(world);
  if (!basePath.empty()) {
    Point2D catGoal = basePath.front();
    if (world->catCanMoveToPosition(catGoal)) {
      //cat is about to win!! stop it!!!
      std::cout <<"CATCHER: CAT IS ABOUT TO WIN. BLOCKING END\n";
      return catGoal;
    }

    if (moveStrategy == VARIABLE) {
      int distance = basePath.size();
      int centerDistance = abs(cat.x) + abs(cat.y);
      std::cout<< "Catcher info: Side " << side << " cat distance " << distance << " center distance " << centerDistance <<"\n";
      int catNeighbors = NeighborCount(world,cat);
      if (catNeighbors >= 3) {
        std::cout <<"CATCHER: CAT HAS MANY NEIGHBORS, CHOOSING LONGEST + NEIGHBOR PRIORITY\n";
        prioritizeNeighbors = true;
        moveStrategy = LONGEST_PATH;

      }else {
        if (distance < 4) {
          std::cout <<"CATCHER: CHOOSING HOLE\n";
          moveStrategy = HOLE_WALL;
        }else {
          std::cout <<"CATCHER: CHOOSING LONGEST\n";
          moveStrategy = LONGEST_PATH;
          if (distance >= side && centerDistance > side-4) {
            std::cout <<"CATCHER: HIGH DISTANCE, FAVORING NEIGHBORS\n";
            prioritizeNeighbors = true;

          }

        }

      }
    }

    if (moveStrategy == HOLE_WALL) {

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

    if (moveStrategy == LONGEST_PATH) {

      std::vector<Point2D> possibleMoves;
      possibleMoves.push_back(basePath.front());
      possibleMoves.push_back(World::NE(cat));
      possibleMoves.push_back(World::NW(cat));
      possibleMoves.push_back(World::E(cat));
      possibleMoves.push_back(World::W(cat));
      possibleMoves.push_back(World::SE(cat));
      possibleMoves.push_back(World::SW(cat));

      int longestLength = 0;
      bool isNeighbor = false;
      Point2D longestMove;
      for (auto m : possibleMoves) {
        if (world->catcherCanMoveToPosition(m) && !world->getContent(m)) {
          std::vector<Point2D> altPath = generatePath(world,m);
          if (altPath.empty()) {
            std::cout <<"CATCHER: ALT PATH HAS NO SOLUTION.\n";
            return m;
          }
          std::cout <<"CATCHER CHECKING ALT PATH: " << world->catCanMoveToPosition(m) << " " << altPath.size() <<"\n";
          bool neighborBenefit = isNeighbor;
          if (!prioritizeNeighbors) {
            neighborBenefit = !neighborBenefit;
          }
          if (altPath.size() > longestLength || (altPath.size() == longestLength && !neighborBenefit )) {
            longestLength = altPath.size();
            longestMove = m;
            isNeighbor = world->catCanMoveToPosition(m);
          }
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
