#include "turing_machine_simulator_app.h"

namespace turingmachinesimulator {

TuringMachineSimulatorApp::TuringMachineSimulatorApp() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void TuringMachineSimulatorApp::draw() {
  // set background color to white
  ci::gl::color(ci::Color("white"));
  glm::vec2 upper_corner = glm::vec2(0,0);
  glm::vec2 lower_corner = glm::vec2(kWindowSize, kWindowSize);
  ci::gl::drawSolidRect(ci::Rectf(upper_corner, lower_corner));
  
  // draw menu bar rectangle
  ci::gl::color(ci::Color("ghostwhite"));
  glm::vec2 upper_corner2 = glm::vec2(kMenuXBoundary, 0);
  glm::vec2 lower_corner2 = glm::vec2(kWindowSize, kWindowSize);
  ci::gl::drawSolidRect(ci::Rectf(upper_corner2, lower_corner2));
  
  // add example states to menu
  kExampleStartingState.Display();
  kExampleNthState.Display();
  kExampleHaltingState.Display();
  
  // display user-defined states
  for (const State &state : states_) {
    state.Display();
  }
}

void TuringMachineSimulatorApp::mouseDown(ci::app::MouseEvent event) {
  const glm::vec2 kClickLocation = event.getPos();
  
  // delete the old clicked_state_ node if it is touching an example state
  // this is in a try catch because on the first-click clicked_state_ is undefined
  try {
    // 2 nodes are touching each-other if their distance is within double their 
    // radii
    const int kDoubleTheRadii = 2 * kRadiusOfStates;
    if (clicked_state_.StateCenterIsWithinGivenRadius(kLocationOfQ1,
        kDoubleTheRadii)
        || clicked_state_.StateCenterIsWithinGivenRadius(kLocationOfQn,
        kDoubleTheRadii) ||
        clicked_state_.StateCenterIsWithinGivenRadius(kLocationOfQh,
        kDoubleTheRadii)) {
      // iterate through the states and remove the clicked_state_ node from the
      // list of active states
      for (int i = 0; i < states_.size(); i++) {
        State current_state = states_.at(i);
        if (current_state.GetId() == clicked_state_.GetId()) {
          states_.erase(states_.begin() + i);
          break; // once we've found the state to erase, nothing to search for
        }
      }
    }
  } catch (const std::exception &exception) {} // it doesn't matter if it throws
  
  /*
   * if the user has clicked an example node, create a new node of that type
   * and set the clicked node equal to the new node
   * 
   * NOTE: return statements in the if statements avoid unnecessary
   * execution of the for-loop checking to see if a user-defined state has been
   * clicked on
   */
  if (kExampleStartingState.StateCenterIsWithinGivenRadius(kClickLocation,
      kRadiusOfStates)) {
    State new_state = State(state_id_, "q1", kClickLocation,
        kRadiusOfStates);
    states_.push_back(new_state);
    clicked_state_ = new_state;
    state_id_ += 1;
    return;
  }
  if (kExampleNthState.StateCenterIsWithinGivenRadius(kClickLocation,
      kRadiusOfStates)) {
    State new_state = State(state_id_, "qn", kClickLocation,
        kRadiusOfStates);
    states_.push_back(new_state);
    clicked_state_ = new_state;
    state_id_ += 1;
    return;
  }
  if (kExampleHaltingState.StateCenterIsWithinGivenRadius(kClickLocation,
      kRadiusOfStates)) {
    State new_state = State(state_id_, "qh", kClickLocation,
        kRadiusOfStates);
    states_.push_back(new_state);
    clicked_state_ = new_state;
    state_id_ += 1;
    return;
  }
  
  // if a user defined state is clicked on, it becomes the clicked state
  for (const State &state : states_) {
    if (state.StateCenterIsWithinGivenRadius(kClickLocation, kRadiusOfStates)) {
      clicked_state_ = state;
      break; // once the clicked on state is found, nothing to search for
    }
  }
}

void TuringMachineSimulatorApp::mouseDrag(ci::app::MouseEvent event) {
  for (int i = 0; i < states_.size(); i++) {
    const State kCurrentState = states_.at(i);
    int current_state_id = kCurrentState.GetId();
    // update the clicked state's position to be where the user dragged it
    if (current_state_id == clicked_state_.GetId()) {
      State updated_state = State(current_state_id,
          kCurrentState.GetStateName(), event.getPos(), kRadiusOfStates);
      states_[i] = updated_state;
      clicked_state_ = updated_state;
      break; // once the state to update has been found, nothing to search for
    }
  }
}

} // namespace turingmachinesimulator
