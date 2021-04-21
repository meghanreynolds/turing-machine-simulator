#include "turing_machine_simulator_app.h"

namespace turingmachinesimulator {

TuringMachineSimulatorApp::TuringMachineSimulatorApp() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void TuringMachineSimulatorApp::draw() {
  // set background color to white
  ci::gl::color(ci::Color("white"));
  const glm::vec2 kUpperScreenCorner = glm::vec2(0,0);
  const glm::vec2 kLowerScreenCorner = glm::vec2(kWindowSize, kWindowSize);
  ci::gl::drawSolidRect(ci::Rectf(kUpperScreenCorner, kLowerScreenCorner));
  
  // draw menu bar rectangle
  ci::gl::color(ci::Color("ghostwhite"));
  const glm::vec2 kMenuUpperCorner = glm::vec2(kMenuXBoundary, 0);
  const glm::vec2 kMenuLowerCorner = glm::vec2(kWindowSize, kWindowSize);
  ci::gl::drawSolidRect(ci::Rectf(kMenuUpperCorner, kMenuLowerCorner));
  
  // display example states to menu
  kExampleStartingState.Display();
  kExampleNthState.Display();
  kExampleHaltingState.Display();
  
  // display add arrow menu
  DrawAddArrowMenu();
  
  // display the clear button
  DrawClearButton();

  // display user-defined directions
  // NOTE: this must come before displaying user-defined states so the arrow 
  // goes behind the states
  for (const Direction &kDirection : directions_) {
    DrawArrow(kDirection);
  }
  
  // display user-defined states
  for (const State &kState : states_) {
    kState.Display();
  }
}

void TuringMachineSimulatorApp::mouseDown(ci::app::MouseEvent event) {
  const glm::vec2 kClickLocation = event.getPos();
  
  // handle clicked box event
  const bool kBoxWasClicked = HandleClickedBox(kClickLocation);
  if (kBoxWasClicked) {
    clicked_state_ = State(); // a state was no longer clicked recently
    return;
  }
  
  // check to see if the clicked state has been dragged over an example state
  // to be deleted
  // if clicked state is empty, there is no need to handle this possibility
  if (!clicked_state_.IsEmpty()) {
    const bool kStateWasDeleted = HandleStateDeletion(kClickLocation);
    if (kStateWasDeleted) {
      clicked_state_ = State(); // the clicked state no longer exists
      return;
    }
  }
  
  // handle the creation of new state
  const bool kStateWasCreated = HandleStateCreation(kClickLocation);
  if (kStateWasCreated) {
    return;
  }
  
  // if a user defined state is clicked on, if it was a right click, change the
  // name of that state, or else, it becomes the clicked state
  for (const State &state : states_) {
    if (state.IsStateCenterWithinGivenRadiusOfPoint(kClickLocation,
        kRadiusOfStates)) {
      if (event.isRight()) {
        editing_state_name_ = true;
        state_being_modified_ = state;
      } else {
        clicked_state_ = state;
      }
      return; // once the clicked on state is found, nothing to search for
    }
  }
  
  // if no state gets clicked and no action is preformed, clicked state is empty
  // this is necessary for if a user clicks a state and then clicks the canvas
  clicked_state_ = State(); 
}

void TuringMachineSimulatorApp::mouseDrag(ci::app::MouseEvent event) {
  // if clicked state is empty, there's nothing to drag
  if (clicked_state_.IsEmpty()) {
    return;
  }
  // update the clicked state's position to be where the user dragged it
  // cannot do a for-each loop here because we need the index in states_
  for (size_t i = 0; i < states_.size(); i++) {
    const State kCurrentState = states_.at(i);
    // update the clicked state's position to be where the user dragged it
    if (kCurrentState.Equals(clicked_state_)) {
      const State kUpdatedState = State(kCurrentState.GetId(),
          kCurrentState.GetStateName(), event.getPos(), kRadiusOfStates);
      states_[i] = kUpdatedState;
      clicked_state_ = states_[i];
      break; // once the state to update has been found, nothing to search for
    }
  }
}

void TuringMachineSimulatorApp::keyDown(ci::app::KeyEvent event) {
  const int kEventCode = event.getCode();
  // NOTE: shift keys will cause major bugs in this ui if not explicitly handled
  if (kEventCode == ci::app::KeyEvent::KEY_LSHIFT 
      || kEventCode == ci::app::KeyEvent::KEY_RSHIFT) {
    return;
  }
  // editing state name takes precedence to adding arrow if both are in progress
  if (editing_state_name_) {
    EditStateName(event);
    return;
  }
  
  const size_t kNotEditingAddArrowText= 5;
  if (index_of_add_arrow_text_to_edit != kNotEditingAddArrowText) {
    EditAddArrowInputBox(event);
  }
}

void TuringMachineSimulatorApp::DrawAddArrowMenu() const {
  // draw the add arrow box
  ci::gl::color(ci::Color("papayawhip"));
  const glm::vec2 kUpperCorner= glm::vec2(kAddArrowBoxXBoundary,
      kAddArrowBoxYBoundary);
  const glm::vec2 kLowerCorner = glm::vec2(kWindowSize, kWindowSize);
  ci::gl::drawSolidRect(ci::Rectf(kUpperCorner, kLowerCorner));
  
  //outline the box
  ci::gl::color(ci::Color("black"));
  ci::gl::drawStrokedRect(ci::Rectf(kUpperCorner, kLowerCorner));
  
  // draw menu heading
  const float kXLocationOfHeading = ((kWindowSize - kAddArrowBoxXBoundary) / 2) 
      + kAddArrowBoxXBoundary;
  const glm::vec2 kLocationOfHeading = glm::vec2(kXLocationOfHeading, 
      kAddArrowBoxYBoundary + 5);
  ci::gl::drawStringCentered("ADD ARROW", kLocationOfHeading, 
      ci::Color("black"));
  
  // add input for read
  const glm::vec2 kLocationOfRead = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 20);
  ci::gl::drawString("Read: ", kLocationOfRead, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kReadInputBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfReadInput], 
      kUpperCornerOfReadInput, ci::Color("black"));
  
  // add input for write
  const glm::vec2 kLocationOfWrite = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 45);
  ci::gl::drawString("Write: ", kLocationOfWrite, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kWriteInputBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfWriteInput],
      kUpperCornerOfWriteInput, ci::Color("black"));
  
  // add input for shift
  const glm::vec2 kLocationOfShift = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 70);
  ci::gl::drawString("Shift: ", kLocationOfShift, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kShiftInputBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfShiftInput], 
      kUpperCornerOfShiftInput, ci::Color("black"));
  
  // add input for move from
  const glm::vec2 kLocationOfMoveFrom = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 95);
  ci::gl::drawString("Move From: ", kLocationOfMoveFrom, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kMoveFromInputBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfMoveFromInput], 
      kUpperCornerOfMoveFromInput, ci::Color("black"));
  
  // add input for move to
  const glm::vec2 kLocationOfMoveTo = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 120);
  ci::gl::drawString("Move To: ", kLocationOfMoveTo, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kMoveToInputBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfMoveToInput], 
      kUpperCornerOfMoveToInput, ci::Color("black"));
  
  //add submit button
  ci::gl::color(ci::Color("honeydew"));
  ci::gl::drawSolidRect(kSubmitButton);
  const glm::vec2 kLocationOfSubmit = glm::vec2(kXLocationOfHeading, 
      kAddArrowBoxYBoundary + 140);
  ci::gl::drawStringCentered("SUBMIT", kLocationOfSubmit, 
      ci::Color("black"));
}

void TuringMachineSimulatorApp::DrawClearButton() const {
  ci::gl::color(ci::Color("salmon"));
  ci::gl::drawSolidRect(kClearButton);
  ci::gl::color(ci::Color("black"));
  ci::gl::drawStrokedRect(kClearButton);
  const glm::vec2 kClearButtonLabelCoordinates = glm::vec2(50, 50);
  ci::gl::drawStringCentered("CLEAR", kClearButtonLabelCoordinates, 
      "black");
}

void TuringMachineSimulatorApp::DrawArrow(const Direction &kDirection) const {
  glm::vec2 move_from_center;
  glm::vec2 move_to_center;
  for (const State &kState : states_) {
    // NOTE: not in an else-if statement because a direction may keep the same 
    // state, in which case the move from and move to states would be the same
    if (kState.Equals(kDirection.GetStateToMoveFrom())) {
      move_from_center = kState.GetStateLocation();
    }
    if (kState.Equals(kDirection.GetStateToMoveTo())) {
      move_to_center = kState.GetStateLocation();
    }
  }
  ci::gl::color(ci::Color("black"));
  ci::gl::drawLine(move_from_center, move_to_center);
  const glm::vec2 kLocationOfDirectionText = 
      kHelperMethods.GetDirectionTextLocation(move_from_center, move_to_center, 
      kRadiusOfStates);
  ci::gl::drawString(kDirection.ToString(), kLocationOfDirectionText, 
      "black");
}

bool TuringMachineSimulatorApp::HandleClickedBox(const glm::vec2
    &kClickLocation) {
  const size_t kIndexOfNoTextToEdit = 5;
  bool input_box_was_clicked = false;
  if (kHelperMethods.IsPointInRectangle(kClickLocation, kReadInputBox)) {
    index_of_add_arrow_text_to_edit = kIndexOfReadInput;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation,
      kWriteInputBox)) {
    index_of_add_arrow_text_to_edit = kIndexOfWriteInput;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation,
      kShiftInputBox)) {
    index_of_add_arrow_text_to_edit = kIndexOfShiftInput;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation,
      kMoveFromInputBox)) {
    index_of_add_arrow_text_to_edit = kIndexOfMoveFromInput;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation,
      kMoveToInputBox)) {
    index_of_add_arrow_text_to_edit = kIndexOfMoveToInput;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation, kSubmitButton)) {
    // on submit create a direction from the inputted information
    Direction direction = Direction(add_arrow_inputs_, states_);
    // if direction is empty, the creation of a direction was unsuccessful, so
    // we don't want to add it to the list of directions
    if (!direction.IsEmpty()) {
      directions_.push_back(direction);
      DrawArrow(direction);
    }
    index_of_add_arrow_text_to_edit = kIndexOfNoTextToEdit;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation, kClearButton)) {
    // reset all modifiable global variables when clear is clicked
    directions_ = {};
    states_ = {};
    state_id_ = 0;
    add_arrow_inputs_ = {"single char", "single char", "L/R/N", "q5", "qh"};
    index_of_add_arrow_text_to_edit = 5;
    editing_state_name_ = false;
    state_being_modified_ = State();
  }
  return input_box_was_clicked;
}

bool TuringMachineSimulatorApp::HandleStateCreation(const glm::vec2 
    &kClickLocation) {
  bool new_state_was_created = false;
  State new_state;
  if (kExampleStartingState.IsStateCenterWithinGivenRadiusOfPoint(kClickLocation, 
      kRadiusOfStates)) {
    new_state = State(state_id_, "q1", kClickLocation, 
        kRadiusOfStates);
    new_state_was_created = true;
  }
  if (kExampleNthState.IsStateCenterWithinGivenRadiusOfPoint(kClickLocation,
      kRadiusOfStates)) {
    new_state = State(state_id_, "qn", kClickLocation,
       kRadiusOfStates);
    new_state_was_created = true;
  }
  if (kExampleHaltingState.IsStateCenterWithinGivenRadiusOfPoint(kClickLocation,
      kRadiusOfStates)) {
    new_state = State(state_id_, "qh", kClickLocation, 
        kRadiusOfStates);
    new_state_was_created = true;
  }
  if (new_state_was_created) {
    states_.push_back(new_state);
    state_id_ += 1;
    clicked_state_ = new_state;
  }
  return new_state_was_created;
}

bool TuringMachineSimulatorApp::HandleStateDeletion(const glm::vec2 
    &kClickLocation) {
  // 2 nodes are touching each-other if their distance is within double their 
  // radii
  const int kDoubleTheRadii = 2 * kRadiusOfStates;
  if (clicked_state_.IsStateCenterWithinGivenRadiusOfPoint(kLocationOfQ1,
      kDoubleTheRadii) || clicked_state_.IsStateCenterWithinGivenRadiusOfPoint
      (kLocationOfQn, kDoubleTheRadii) || 
      clicked_state_.IsStateCenterWithinGivenRadiusOfPoint(kLocationOfQh,
      kDoubleTheRadii)) {
    // iterate through the states and remove the clicked_state_ node from the
    // list of active states
    State state_to_erase;
    // need to use for-each loop here because we need the index of the state
    // to delete
    for (size_t i = 0; i < states_.size(); i++) {
      State current_state = states_.at(i);
      if (current_state.Equals(clicked_state_)) {
        state_to_erase = current_state;
        states_.erase(states_.begin() + i);
        break; // once we've found the state to erase, nothing to search for
      }
    }
    
    // iterate through the directions and remove all directions involving
    // the deleted state
    const std::vector<Direction> kOriginalDirections = directions_;
    size_t index_in_directions = 0;
    for (size_t i = 0; i < kOriginalDirections.size(); i++) {
      Direction current_direction = kOriginalDirections.at(i);
      // NOTE: Must be in an else if statement in case state to move to is the
      // same as the state to move from
      if (current_direction.GetStateToMoveTo().Equals(state_to_erase)) {
        directions_.erase(directions_.begin() + index_in_directions);
        index_in_directions -= 1;
      } else if (current_direction.GetStateToMoveFrom().Equals(state_to_erase)) {
        directions_.erase(directions_.begin() + index_in_directions);
        index_in_directions -= 1;
      }
      index_in_directions += 1;
    }
    return true;
  }
  return false;
}

void TuringMachineSimulatorApp::EditStateName(const ci::app::KeyEvent 
    &kKeyEvent) {
  const int kEventCode = kKeyEvent.getCode();
  // on return, stop editing the state name
  if (kEventCode == ci::app::KeyEvent::KEY_RETURN) {
    editing_state_name_ = false;
    state_being_modified_ = State();
    return;
  }
  
  // find the index in states_ of the state whose name is being modified
  size_t kIndexOfStateBeingModified;
  for (size_t i = 0; i < states_.size(); i++) {
    const State kCurrentState = states_.at(i);
    if (kCurrentState.Equals(state_being_modified_)) {
      kIndexOfStateBeingModified = i;
      break; // break because there's nothing left to search for
    }
  }
  const std::string kNameToEdit = state_being_modified_.GetStateName();
  
  // remove the last character from the string on backspace
  if (kEventCode == ci::app::KeyEvent::KEY_BACKSPACE) {
    // state name may not be less than 1 character (the q may not be deleted)
    if (kNameToEdit.size() > 1) {
      const size_t kLastCharacterOfName = kNameToEdit.size() - 1;
      const std::string kUpdatedName = kNameToEdit.substr(0,
          kLastCharacterOfName);
      state_being_modified_.SetStateName(kUpdatedName);
      states_[kIndexOfStateBeingModified] = state_being_modified_;
    }
    return;
  }
  
  // if no special event codes were typed, add the character that was typed
  // to the end of the state's name
  state_being_modified_.SetStateName(kNameToEdit + kKeyEvent.getChar());
  states_[kIndexOfStateBeingModified] = state_being_modified_;
  return;
}

void TuringMachineSimulatorApp::EditAddArrowInputBox(const ci::app::KeyEvent 
    &kKeyEvent) {
  const int kEventCode = kKeyEvent.getCode();
  
  // on return, stop editing the text that's being edited
  if (kEventCode == ci::app::KeyEvent::KEY_RETURN) {
    const size_t kNotEditingAddArrowText= 5;
    index_of_add_arrow_text_to_edit = kNotEditingAddArrowText;
    return;
  }

  const std::string kTextToEdit =
      add_arrow_inputs_[index_of_add_arrow_text_to_edit];
  // if backspace, remove 1 character from the end of the text being edited
  if (kEventCode == ci::app::KeyEvent::KEY_BACKSPACE) {
    // if size of text is less than or equal to 0, there's nothing to remove
    if (kTextToEdit.size() > 0) {
      const size_t kLastCharacterOfText = kTextToEdit.size() - 1;
      add_arrow_inputs_[index_of_add_arrow_text_to_edit] = 
          kTextToEdit.substr(0, kLastCharacterOfText);
    }
    return;
  }

  // if no special event codes were typed, then append the entered character to 
  // the text that's being edited
  add_arrow_inputs_[index_of_add_arrow_text_to_edit] = kTextToEdit
      + kKeyEvent.getChar();
}

} // namespace turingmachinesimulator
