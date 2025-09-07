#include "CohesionRule.h"
#include "../gameobjects/Boid.h"

Vector2f CohesionRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f cohesionForce;
  Vector2f center;

  // todo: add your code here to make a force towards the center of mass
  // hint: iterate over the neighborhood

  // find center of mass

  for (int i = 0; i < neighborhood.size(); i++) {
    Boid* b = neighborhood[i];
    center += b->getPosition();
  }
  if (!neighborhood.empty()) {
    center /= neighborhood.size();
    double cmdistance = boid->getPosition().Distance(center);
    cohesionForce = center - boid->getPosition();
    return cohesionForce.normalized();
  }else {
    return Vector2f(0,0);
  }
}