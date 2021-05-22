#include "state.h"

namespace turingmachinesimulator {

State::State(int id, const std::string &state_name, const glm::vec2 
    &state_location, double radius, const std::vector<std::string> 
    &possible_halting_state_names)
    : id_(id), 
      state_name_(state_name), 
      state_location_(state_location), 
      radius_(radius),
      possible_halting_state_names_(possible_halting_state_names) {
  // states are only empty when created with the default constructor
  is_empty_ = false;
}

int State::GetId() const {
  return id_;
}
    
void State::SetStateName(const std::string &state_name) {
  state_name_ = state_name;
}

std::string State::GetStateName() const {
  return state_name_;
}

double State::GetRadius() const {
  return radius_;
}

void State::SetStateLocation(const glm::vec2 &state_location) {
  state_location_ = state_location;
}

glm::vec2 State::GetStateLocation() const {
  return state_location_;
}

bool State::IsEmpty() const {
  return is_empty_;
}

bool State::operator<(const State &state) const {
  return id_ < state.GetId();
}

bool State::Equals(const State &state) const {
  return state.GetId() == id_;
}

bool State::IsStateCenterWithinGivenRadiusOfPoint(const glm::vec2 &point,
    double radius) const {
  const float kDistanceBetweenStateCenterAndPoint = glm::distance(
      state_location_, point);
  if (kDistanceBetweenStateCenterAndPoint <= radius) {
    return true;
  }
  return false;
}

void State::Display() const {
  const std::string kStartingStateName = "q1";
  if (state_name_ == kStartingStateName) {
    DrawStartingState();
  } else if (std::find(possible_halting_state_names_.begin(), 
      possible_halting_state_names_.end(), state_name_) 
      != possible_halting_state_names_.end()) {
    DrawHaltingState(state_name_);
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

void State::DrawHaltingState(const std::string &name) const {
  ci::gl::color(ci::Color("mediumspringgreen"));
  ci::gl::drawSolidCircle(state_location_, radius_);
  ci::gl::color(ci::Color("white"));
  // radii decrementing in 2's (-2, -4, -6) was found to yield the most visually
  // appealing radii for the inner circles of the halting states
  ci::gl::drawSolidCircle(state_location_, (radius_ - 2));
  ci::gl::color(ci::Color("mediumspringgreen"));
  ci::gl::drawSolidCircle(state_location_, (radius_ - 4));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidCircle(state_location_, (radius_ - 6));
  ci::gl::color(ci::Color("black"));
  ci::gl::drawStringCentered(name, state_location_,
      ci::Color("black"));
}

} // namespace turingmachinesimulator
