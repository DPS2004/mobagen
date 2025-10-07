#ifndef CATCHER_H
#define CATCHER_H

#include "Agent.h"

class Catcher : public Agent {
public:
  explicit Catcher() : Agent(){};
  Point2D Move(World*) override;
  int NeighborCount(World* w, Point2D p);
  enum strategy {
    HOLE_WALL,
    LONGEST_PATH
  };
  strategy currentStrategy = LONGEST_PATH;
};

#endif  // CATCHER_H
