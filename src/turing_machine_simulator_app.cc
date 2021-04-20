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
  glm::vec2 upper_corner_menu = glm::vec2(kMenuXBoundary, 0);
  glm::vec2 lower_corner_menu = glm::vec2(kWindowSize, kWindowSize);
  ci::gl::drawSolidRect(ci::Rectf(upper_corner_menu, lower_corner_menu));
  
  // add example states to menu
  kExampleStartingState.Display();
  kExampleNthState.Display();
  kExampleHaltingState.Display();
  
  // display the add arrow menu
  DrawAddArrowMenu();
  
  // display the clear button
  DrawClearButton();

  // display user-defined directions (NOTE: this must come before displaying
  // user-defined states so the arrow goes behind the states)
  for (const Direction &direction : directions_) {
    DrawArrow(direction);
  }
  
  // display user-defined states
  for (const State &state : states_) {
    state.Display();
  }
}

void TuringMachineSimulatorApp::mouseDown(ci::app::MouseEvent event) {
  const glm::vec2 kClickLocation = event.getPos();
  // handle clicked boxes
  const bool kBoxWasClicked = HandleClickedBox(kClickLocation);
  if (kBoxWasClicked) {
    clicked_state_ = State();
    return;
  }
  // check to see if the clicked state has been dragged over an example state
  // to be deleted, if clicked state is empty, there is no need to handle 
  // this possibility
  if (!clicked_state_.IsEmpty()) {
    const bool kStateWasDeleted = HandleStateDeletion(kClickLocation);
    if (kStateWasDeleted) {
      clicked_state_ = State();
      return;
    }
  }
  // handle creation of new state
  const bool kStateWasCreated = HandleStateCreation(kClickLocation);
  if (kStateWasCreated) {
    return;
  }
  // if a user defined state is clicked on, if it was a right click, change the
  // name of that state, else, it becomes the clicked state
  for (const State &state : states_) {
    if (state.StateCenterIsWithinGivenRadius(kClickLocation, kRadiusOfStates)) {
      if (event.isRight()) {
        state_being_modified_ = state;
        editing_state_name_ = true;
      } else {
        clicked_state_ = state;
      }
      return; // once the clicked on state is found, nothing to search for
    }
  }
  clicked_state_ = State();
}

void TuringMachineSimulatorApp::mouseDrag(ci::app::MouseEvent event) {
  for (int i = 0; i < states_.size(); i++) {
    const State kCurrentState = states_.at(i);
    // update the clicked state's position to be where the user dragged it
    if (kCurrentState.Equals(clicked_state_)) {
      State updated_state = State(kCurrentState.GetId(),
          kCurrentState.GetStateName(), event.getPos(), kRadiusOfStates);
      states_[i] = updated_state;
      clicked_state_ = updated_state;
      break; // once the state to update has been found, nothing to search for
    }
  }
}

void TuringMachineSimulatorApp::keyDown(ci::app::KeyEvent event) {
  // editing state name takes precedence to adding arrow
  if (editing_state_name_) {
    if (event.getCode() == ci::app::KeyEvent::KEY_RETURN) {
      editing_state_name_ = false;
      return;
    }

    const std::string kNameToEdit = state_being_modified_.GetStateName();
    size_t kIndexOfStateBeingModified;
    for (size_t i = 0; i < states_.size(); i++) {
      const State kCurrentState = states_.at(i);
      if (kCurrentState.Equals(state_being_modified_)) {
        kIndexOfStateBeingModified = i;
        break;
      }
    }
    
    if (event.getCode() == ci::app::KeyEvent::KEY_BACKSPACE) {
      // state name may not be les than 1 character
      if (kNameToEdit.size() > 1) {
        const size_t kLastCharacterOfName = kNameToEdit.size() - 1;
        const std::string kUpdatedName = kNameToEdit.substr(0, 
            kLastCharacterOfName);
        state_being_modified_.SetStateName(kUpdatedName);
        states_[kIndexOfStateBeingModified] = state_being_modified_;
      }
      return;
    }
    
    state_being_modified_.SetStateName(kNameToEdit + event.getChar());
    states_[kIndexOfStateBeingModified] = state_being_modified_;
    return;
  }
  
  const size_t kNotTakingKeyboardInputValue= 5;
  if (index_of_add_arrow_text_to_edit == kNotTakingKeyboardInputValue) {
    return;
  }
  // when the user clicks enter stop looking for keyboard input (they are done
  // editing the text)
  if (event.getCode() == ci::app::KeyEvent::KEY_RETURN) {
    index_of_add_arrow_text_to_edit = 5;
    return;
  }

  // if backspace, remove 1 character from the end of the text being edited
  if (event.getCode() == ci::app::KeyEvent::KEY_BACKSPACE) {
    const std::string kTextToEdit =
        add_arrow_inputs_[index_of_add_arrow_text_to_edit];
    // if size of text is less than or equal to 0, there's nothing to remove
    if (kTextToEdit.size() > 0) {
      const size_t kLastCharacterOfText = kTextToEdit.size() - 1;
      add_arrow_inputs_[index_of_add_arrow_text_to_edit] =
          kTextToEdit.substr(0, kLastCharacterOfText);
    }
    return;
  }

  // if not a special key, then append the entered character to the text
  const std::string kTextToEdit =
      add_arrow_inputs_[index_of_add_arrow_text_to_edit];
  add_arrow_inputs_[index_of_add_arrow_text_to_edit] = kTextToEdit
      + event.getChar();
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
  
  // write menu heading
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
  ci::gl::drawString(add_arrow_inputs_[0], kUpperCornerOfReadInput,
      ci::Color("black"));
  
  // add input for write
  const glm::vec2 kLocationOfWrite = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 45);
  ci::gl::drawString("Write: ", kLocationOfWrite, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kWriteInputBox);
  ci::gl::drawString(add_arrow_inputs_[1], kUpperCornerOfWriteInput,
      ci::Color("black"));
  
  // add input for shift
  const glm::vec2 kLocationOfShift = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 70);
  ci::gl::drawString("Shift: ", kLocationOfShift, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kShiftInputBox);
  ci::gl::drawString(add_arrow_inputs_[2], kUpperCornerOfShiftInput,
      ci::Color("black"));
  
  // add input for move from
  const glm::vec2 kLocationOfMoveFrom = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 95);
  ci::gl::drawString("Move From: ", kLocationOfMoveFrom, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kMoveFromInputBox);
  ci::gl::drawString(add_arrow_inputs_[3], kUpperCornerOfMoveFromInput,
      ci::Color("black"));
  
  // add input for move to
  const glm::vec2 kLocationOfMoveTo = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 120);
  ci::gl::drawString("Move To: ", kLocationOfMoveTo, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kMoveToInputBox);
  ci::gl::drawString(add_arrow_inputs_[4], kUpperCornerOfMoveToInput,
      ci::Color("black"));
  
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
  for (const State &state : states_) {
    if (state.Equals(kDirection.GetStateToMoveFrom())) {
      move_from_center = state.GetStateLocation();
    }
    if (state.Equals(kDirection.GetStateToMoveTo())) {
      move_to_center = state.GetStateLocation();
    }
  }
  ci::gl::color(ci::Color("black"));
  ci::gl::drawLine(move_from_center, move_to_center);
}

bool TuringMachineSimulatorApp::HandleClickedBox(const glm::vec2
    &kClickLocation) {
  const TuringMachineSimulatorHelperMethods kHelperMethods =
      TuringMachineSimulatorHelperMethods();
  bool input_box_was_clicked = false;
  if (kHelperMethods.IsPointInRectangle(kClickLocation, kReadInputBox)) {
    index_of_add_arrow_text_to_edit = 0;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation,
      kWriteInputBox)) {
    index_of_add_arrow_text_to_edit = 1;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation,
      kShiftInputBox)) {
    index_of_add_arrow_text_to_edit = 2;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation,
      kMoveFromInputBox)) {
    index_of_add_arrow_text_to_edit = 3;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation,
      kMoveToInputBox)) {
    index_of_add_arrow_text_to_edit = 4;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation, kSubmitButton)) {
    Direction direction = Direction(add_arrow_inputs_, states_);
    if (!direction.IsEmpty()) {
      directions_.push_back(direction);
      DrawArrow(direction);
    }
    index_of_add_arrow_text_to_edit = -1;
    input_box_was_clicked = true;
  } else if (kHelperMethods.IsPointInRectangle(kClickLocation, kClearButton)) {
    // reset all modifiable global variables on clear
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
  if (kExampleStartingState.StateCenterIsWithinGivenRadius(kClickLocation,
      kRadiusOfStates)) {
    new_state = State(state_id_, "q1", kClickLocation,
        kRadiusOfStates);
    new_state_was_created = true;
  }
  if (kExampleNthState.StateCenterIsWithinGivenRadius(kClickLocation,
      kRadiusOfStates)) {
    new_state = State(state_id_, "qn", kClickLocation,
       kRadiusOfStates);
    new_state_was_created = true;
  }
  if (kExampleHaltingState.StateCenterIsWithinGivenRadius(kClickLocation,
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
  if (clicked_state_.StateCenterIsWithinGivenRadius(kLocationOfQ1,
      kDoubleTheRadii) || clicked_state_.StateCenterIsWithinGivenRadius
      (kLocationOfQn, kDoubleTheRadii) ||
      clicked_state_.StateCenterIsWithinGivenRadius(kLocationOfQh,
      kDoubleTheRadii)) {
    // iterate through the states and remove the clicked_state_ node from the
    // list of active states
    State state_to_erase;
    for (size_t i = 0; i < states_.size(); i++) {
      State current_state = states_.at(i);
      if (current_state.GetId() == clicked_state_.GetId()) {
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
      if (current_direction.GetStateToMoveTo().Equals(state_to_erase)) {
        directions_.erase(directions_.begin() + index_in_directions);
        index_in_directions -= 1;
      }
      if (current_direction.GetStateToMoveFrom().Equals(state_to_erase)) {
        directions_.erase(directions_.begin() + index_in_directions);
        index_in_directions -= 1;
      }
      index_in_directions += 1;
    }
    return true;
  }
  return false;
}

} // namespace turingmachinesimulator
