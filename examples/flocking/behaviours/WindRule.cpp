#include "WindRule.h"
#include <imgui.h>
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"
#include "engine/Engine.h"

Vector2f WindRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  // todo: add a wind force here
  // hint: use the windAngle variable
  Vector2f force = Vector2f::up();
  force = force.Rotate(windAngle * (180.0 / M_PI));
  return force;
}

bool WindRule::drawImguiRuleExtra() {
  ImGui::SetCurrentContext(world->engine->window->imGuiContext);
  bool valueHasChanged = false;

  if (ImGui::SliderAngle("Wind Direction", &windAngle, 0)) {
    valueHasChanged = true;
  }

  return valueHasChanged;
}