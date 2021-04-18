#include "state.h"

namespace turingmachinesimulator {

State::State(int id, const std::string &kStateName, 
    const glm::vec2 &kStateLocation, double radius) 
    : id_(id), state_name_(kStateName), state_location_(kStateLocation), 
    radius_(radius) {}
    
void State::SetStateName(const std::string &kStateName) {
  state_name_ = kStateName;
}

std::string State::GetStateName() const {
  return state_name_;
}

void State::SetStateLocation(const glm::vec2 &kStateLocation) {
  state_location_ = kStateLocation;
}

glm::vec2 State::GetStateLocation() const {
  return state_location_;
}

int State::GetId() const {
  return id_;
}

bool State::StateCenterIsWithinGivenRadius(const glm::vec2 &kLocation,
    double radius) const {
  const float kDistanceBetweenStateAndPoint = glm::distance(state_location_, 
      kLocation);
  if (kDistanceBetweenStateAndPoint <= radius) {
    return true;
  }
  return false;
}

void State::Display() const {
  if (state_name_ == "q1") {
    DrawStartingState();
  } else if (state_name_ == "qh") {
    DrawHaltingState();
  } else {
    DrawNthState();
  }
}

void State::DrawStartingState() const {
  ci::gl::color(ci::Color("plum"));
  ci::gl::drawSolidCircle(state_location_, radius_);
  ci::gl::color(ci::Color("white"));
  // radius - 2 was found to be the most visually appealing radius for the inner
  // circle
  ci::gl::drawSolidCircle(state_location_, (radius_ - 2));
  ci::gl::color(ci::Color("black"));
  ci::gl::drawStringCentered("q1", state_location_,
      ci::Color("black"));
}

void State::DrawNthState() const {
  ci::gl::color(ci::Color("salmon"));
  ci::gl::drawSolidCircle(state_location_, radius_);
  ci::gl::color(ci::Color("white"));
  // radius - 2 was found to be the most visually appealing radius for the inner
  // circle
  ci::gl::drawSolidCircle(state_location_, (radius_ - 2));
  ci::gl::color(ci::Color("black"));
  ci::gl::drawStringCentered(state_name_, state_location_,
      ci::Color("black"));
}

void State::DrawHaltingState() const {
  ci::gl::color(ci::Color("mediumspringgreen"));
  ci::gl::drawSolidCircle(state_location_, radius_);
  ci::gl::color(ci::Color("white"));
  // radii decrementing in 2's (-2, -4, -6) was found to yield the most visually
  // appealing radii for the inner circles
  ci::gl::drawSolidCircle(state_location_, (radius_ - 2));
  ci::gl::color(ci::Color("mediumspringgreen"));
  ci::gl::drawSolidCircle(state_location_, (radius_ - 4));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidCircle(state_location_, (radius_ - 6));
  ci::gl::color(ci::Color("black"));
  ci::gl::drawStringCentered("qh", state_location_,
      ci::Color("black"));
}

} // namespace turingmachinesimulator
