#include "turing_machine_simulator_app.h"

namespace turingmachinesimulator {

TuringMachineSimulatorApp::TuringMachineSimulatorApp() {
  ci::app::setWindowSize(kHorizontalWindowSize, kVerticalWindowSize);
}

void TuringMachineSimulatorApp::draw() {
  // set background color to white
  ci::gl::color(ci::Color("white"));
  const glm::vec2 kUpperScreenCorner = glm::vec2(0,0);
  const glm::vec2 kLowerScreenCorner = glm::vec2(kHorizontalWindowSize,
      kVerticalWindowSize);
  ci::gl::drawSolidRect(ci::Rectf(kUpperScreenCorner, kLowerScreenCorner));
  
  // draw menu bar rectangle
  ci::gl::color(ci::Color("ghostwhite"));
  const glm::vec2 kMenuUpperCorner = glm::vec2(kMenuXBoundary, 0);
  const glm::vec2 kMenuLowerCorner = glm::vec2(kHorizontalWindowSize, 
      kVerticalWindowSize);
  ci::gl::drawSolidRect(ci::Rectf(kMenuUpperCorner, kMenuLowerCorner));
  
  // display example states to menu
  kExampleStartingState.Display();
  kExampleNthState.Display();
  kExampleHaltingState.Display();
  
  // display add arrow menu
  DrawAddArrowMenu();
  
  // display the clear button
  DrawButton(kClearButton, "CLEAR", 
      ci::Color("salmon"));
  
  //display the simulate button
  DrawButton(kSimulateButton, "SIMULATE!",
      ci::Color("palegreen"));
  
  // display the tape
  DrawTape();
  
  // display reset tape button
  DrawButton(kResetTapeButton, "Reset Tape",
      ci::Color("salmon"));
  
  // display stop simulation button if simulation is in progress
  if (simulation_is_in_progress_) {
    DrawButton(kStopSimulationButton, "STOP!",
        ci::Color("salmon"));
  }
  
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

  // display the error message
  // NOTE: This is displayed last so that nothing on the screen covers it up
  DisplayErrorMessage();
}

void TuringMachineSimulatorApp::update() {
  // only use the update function while the simulation is in-progress
  if(simulation_is_in_progress_) {
    // significantly reduce frame rate to allow users to really see how the
    // scanner is moving
    ci::app::setFrameRate(1);
    
    // add the current turing machine configuration to the console output or
    // the markdown file, depending on whether the markdown file exists or not
    std::ofstream configuration_file = 
        std::ofstream(kPathToCompleteConfigurationFile, std::ios::app);
    if (!configuration_file.is_open()) {
      std::cout << turing_machine_.GetConfigurationForConsole();
    } else {
      configuration_file << turing_machine_.GetConfigurationForMarkdown();
    }
    
    // update the turing machine
    turing_machine_.Update();
    tape_ = turing_machine_.GetTape();
    index_of_character_being_read_ = turing_machine_.GetIndexOfScanner();
    if (turing_machine_.IsHalted()) {
      StopSimulation();
    }
  }
}

void TuringMachineSimulatorApp::mouseDown(ci::app::MouseEvent event) {
  const glm::vec2 kClickLocation = event.getPos();
  
  // when simulation is in progress, clicking is not supported unless
  // clicking the stop simulation button
  if (simulation_is_in_progress_) {
    if (turingmachinesimulator::TuringMachineSimulatorHelper::IsPointInRectangle
        (kClickLocation, kStopSimulationButton)) {
      StopSimulation();
    }
    return;
  }
  
  // handle clicked box event
  const bool kBoxWasClicked = HandleClickedBox(kClickLocation);
  if (kBoxWasClicked) {
    clicked_state_ = State(); // a state was no longer clicked recently
    return;
  }
  
  // check to see if the clicked state has been dragged over an example state
  // to be deleted
  // if clicked state is empty, then there is no need to handle this possibility
  if (!clicked_state_.IsEmpty()) {
    const bool kStateWasDeleted = HandleStateDeletion(kClickLocation);
    if (kStateWasDeleted) {
      clicked_state_ = State(); // the clicked state no longer exists
      return;
    }
  }
  
  // handle the creation of a new state
  const bool kStateWasCreated = HandleStateCreation(kClickLocation);
  if (kStateWasCreated) {
    return;
  }
  
  // if a user defined state is clicked on, if it was a right click, change the
  // name of that state, otherwise it becomes the clicked state
  for (const State &kState : states_) {
    if (kState.IsStateCenterWithinGivenRadiusOfPoint(kClickLocation,
        kRadiusOfStates)) {
      if (event.isRight()) {
        editing_state_name_ = true;
        state_being_modified_ = kState;
      } else {
        clicked_state_ = kState;
      }
      return; // once the clicked on state is found, nothing to search for
    }
  }
  
  // if no state gets clicked and no action is performed, clicked state is empty
  // this is necessary for in case a user clicks a state and then clicks the 
  // canvas
  clicked_state_ = State(); 
}

void TuringMachineSimulatorApp::mouseDrag(ci::app::MouseEvent event) {
  // if clicked state is empty or the simulation is in progress, 
  // there's nothing to drag
  if (clicked_state_.IsEmpty() || simulation_is_in_progress_) {
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
  // if simulation is in progress, keyboard input is not supported
  if (simulation_is_in_progress_) {
    return;
  }
  
  const int kEventCode = event.getCode();
  // NOTE: shift keys will cause major bugs in this UI if not explicitly handled
  if (kEventCode == ci::app::KeyEvent::KEY_LSHIFT 
      || kEventCode == ci::app::KeyEvent::KEY_RSHIFT) {
    return;
  }
  // editing tape character takes precedence to all other edits
  if (editing_tape_character_) {
    EditTapeCharacter(event);
    return;
  }
  
  // editing state name takes precedence to adding arrow
  if (editing_state_name_) {
    EditStateName(event);
    return;
  }
  
  // edit add arrow text if applicable
  const size_t kNotEditingAddArrowIndex= 5;
  if (index_of_add_arrow_text_to_edit != kNotEditingAddArrowIndex) {
    EditAddArrowInputBox(event);
  }
}

bool TuringMachineSimulatorApp::HandleClickedBox(const glm::vec2
    &click_location) {
  const size_t kIndexOfNoTextToEdit = 5;
  bool input_box_was_clicked = false;

  if (turingmachinesimulator::TuringMachineSimulatorHelper::IsPointInRectangle
      (click_location, kReadInputBox)) {
    index_of_add_arrow_text_to_edit = kIndexOfReadInput;
    input_box_was_clicked = true;

  } else if (turingmachinesimulator::TuringMachineSimulatorHelper
        ::IsPointInRectangle(click_location, kWriteInputBox)) {
    index_of_add_arrow_text_to_edit = kIndexOfWriteInput;
    input_box_was_clicked = true;

  } else if (turingmachinesimulator::TuringMachineSimulatorHelper
        ::IsPointInRectangle(click_location, kShiftInputBox)) {
    index_of_add_arrow_text_to_edit = kIndexOfShiftInput;
    input_box_was_clicked = true;

  } else if (turingmachinesimulator::TuringMachineSimulatorHelper
        ::IsPointInRectangle(click_location, kMoveFromInputBox)) {
    index_of_add_arrow_text_to_edit = kIndexOfMoveFromInput;
    input_box_was_clicked = true;

  } else if (turingmachinesimulator::TuringMachineSimulatorHelper
        ::IsPointInRectangle(click_location, kMoveToInputBox)) {
    index_of_add_arrow_text_to_edit = kIndexOfMoveToInput;
    input_box_was_clicked = true;

  } else if (turingmachinesimulator::TuringMachineSimulatorHelper
        ::IsPointInRectangle(click_location, kSubmitButton)) {
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

  } else if (turingmachinesimulator::TuringMachineSimulatorHelper
        ::IsPointInRectangle(click_location, kClearButton)) {
    // reset all modifiable global variables when clear is clicked
    directions_ = {};
    states_ = {};
    const char kBlankCharacter = '-';
    tape_ = {kBlankCharacter, kBlankCharacter, kBlankCharacter, kBlankCharacter,
             kBlankCharacter, kBlankCharacter, kBlankCharacter, kBlankCharacter};
    index_of_character_being_read_ = 0;
    simulation_is_in_progress_ = false;
    turing_machine_ = TuringMachine();
    state_id_ = 0;
    add_arrow_inputs_ = {"single char", "single char", "L/R/N", "q5", "qh"};
    index_of_add_arrow_text_to_edit = add_arrow_inputs_.size();
    editing_state_name_ = false;
    state_being_modified_ = State();
    index_of_character_being_edited_ = tape_.size();
    editing_tape_character_ = false;

  } else if (turingmachinesimulator::TuringMachineSimulatorHelper
        ::IsPointInRectangle(click_location, kSimulateButton)) {
    // create a turing machine from user-defined states, directions, and tape
    // on simulate button click
    turing_machine_ = TuringMachine(states_, directions_, tape_);
    // if turing machine is empty, the creation of a turing machine was 
    // unsuccessful so we want to display the error message, otherwise, we
    // want to being the simulation
    if (turing_machine_.IsEmpty()) {
      DisplayErrorMessage();
    } else {
      num_simulations_run_ += 1;
      simulation_is_in_progress_ = true;
      // if complete configurations are being sourced to a markdown file,
      // label this complete configuration with a heading indicating which
      // number simulation this is
      std::ofstream configuration_file =
          std::ofstream(kPathToCompleteConfigurationFile, std::ios::app);
      if (configuration_file.is_open()) {
        // NOTE: "##" is the markdown tag for a secondary heading
        configuration_file << "## Simulation " << num_simulations_run_ 
            << " Complete Configuration:  ##\n";
      }
    }

  } else if (turingmachinesimulator::TuringMachineSimulatorHelper::
        IsPointInRectangle(click_location, kResetTapeButton)) {
    // when the tape is reset, set the tape to all blank characters and 
    // reset scanner to index 0 of the tape
    const char kBlankCharacter = '-';
    tape_ = {kBlankCharacter, kBlankCharacter, kBlankCharacter, kBlankCharacter,
             kBlankCharacter, kBlankCharacter, kBlankCharacter, kBlankCharacter};
    index_of_character_being_read_ = 0;
  }

  // if a square of the tape is clicked, then the user is about to edit
  // the square of the tape that they clicked on
  const size_t kIndexOfTapeSquareClicked = turingmachinesimulator
      ::TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked
      (click_location, tape_.size(), kUpperCornerOfTape,
       kLowerCornerOfTape);
  if (kIndexOfTapeSquareClicked != tape_.size()) {
    editing_tape_character_ = true;
    index_of_character_being_edited_ = kIndexOfTapeSquareClicked;
  }

  return input_box_was_clicked;
}

bool TuringMachineSimulatorApp::HandleStateCreation(const glm::vec2
    &click_location) {
  bool new_state_was_created = false;
  State new_state;

  if (kExampleStartingState.IsStateCenterWithinGivenRadiusOfPoint(click_location,
      kRadiusOfStates)) {
    const std::string kStartingStateName = "q1";
    new_state = State(state_id_, kStartingStateName, click_location,
        kRadiusOfStates);
    new_state_was_created = true;
  }

  if (kExampleNthState.IsStateCenterWithinGivenRadiusOfPoint(click_location,
      kRadiusOfStates)) {
    const std::string kNthStateName = "qn";
    new_state = State(state_id_, kNthStateName, click_location, kRadiusOfStates);
    new_state_was_created = true;
  }

  if (kExampleHaltingState.IsStateCenterWithinGivenRadiusOfPoint(click_location,
      kRadiusOfStates)) {
    const std::string kHaltingStateName = "qh";
    new_state = State(state_id_, kHaltingStateName, click_location,
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
    &click_location) {
  // 2 nodes are touching each-other if their distance is within double their 
  // radii
  const int kDoubleTheRadii = 2 * kRadiusOfStates;
  if (clicked_state_.IsStateCenterWithinGivenRadiusOfPoint(kLocationOfQ1,
      kDoubleTheRadii) || clicked_state_.IsStateCenterWithinGivenRadiusOfPoint
      (kLocationOfQn, kDoubleTheRadii)
      || clicked_state_.IsStateCenterWithinGivenRadiusOfPoint(kLocationOfQh,
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
    for (const Direction &kDirection : kOriginalDirections) {
      // NOTE: Must be in an else if statement in case state to move to is the
      // same as the state to move from
      if (kDirection.GetStateToMoveTo().Equals(state_to_erase)) {
        directions_.erase(directions_.begin() + index_in_directions);
        index_in_directions -= 1;
      } else if (kDirection.GetStateToMoveFrom().Equals(state_to_erase)) {
        directions_.erase(directions_.begin() + index_in_directions);
        index_in_directions -= 1;
      }
      index_in_directions += 1;
    }
    return true;
  }

  return false;
}

void TuringMachineSimulatorApp::EditTapeCharacter(const ci::app::KeyEvent
    &key_event) {
  const int kEventCode = key_event.getCode();

  // on return, stop editing the tape character name
  if (kEventCode == ci::app::KeyEvent::KEY_RETURN) {
    editing_tape_character_ = false;
    index_of_character_being_edited_ = tape_.size();
    return;
  }

  // make the character a blank character on backspace
  if (kEventCode == ci::app::KeyEvent::KEY_BACKSPACE) {
    const char kBlankCharacter = '-';
    tape_[index_of_character_being_edited_] = kBlankCharacter;
    return;
  }

  // if no special event codes were typed, change the character to be the
  // character entered by the user
  tape_[index_of_character_being_edited_] = key_event.getChar();
}


void TuringMachineSimulatorApp::EditStateName(const ci::app::KeyEvent
    &key_event) {
  const int kEventCode = key_event.getCode();

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
  state_being_modified_.SetStateName(kNameToEdit + key_event.getChar());
  states_[kIndexOfStateBeingModified] = state_being_modified_;
}

void TuringMachineSimulatorApp::EditAddArrowInputBox(const ci::app::KeyEvent
    &key_event) {
  const int kEventCode = key_event.getCode();

  // on return, stop editing the text that's being edited
  if (kEventCode == ci::app::KeyEvent::KEY_RETURN) {
    index_of_add_arrow_text_to_edit = add_arrow_inputs_.size();
    return;
  }

  const std::string kTextToEdit =
      add_arrow_inputs_[index_of_add_arrow_text_to_edit];
  // if backspace, remove 1 character from the end of the text being edited
  if (kEventCode == ci::app::KeyEvent::KEY_BACKSPACE) {
    // if size of text is less than or equal to 0, there's nothing to remove
    if (!kTextToEdit.empty()) {
      const size_t kLastCharacterOfText = kTextToEdit.size() - 1;
      add_arrow_inputs_[index_of_add_arrow_text_to_edit] =
          kTextToEdit.substr(0, kLastCharacterOfText);
    }
    return;
  }

  // if no special event codes were typed, then append the entered character to 
  // the text that's being edited
  add_arrow_inputs_[index_of_add_arrow_text_to_edit] = kTextToEdit
      + key_event.getChar();
}

void TuringMachineSimulatorApp::StopSimulation() {
  // add the final turing machine configuration to the console output or
  // the markdown file, depending on whether the markdown file exists or not
  std::ofstream configuration_file =
      std::ofstream(kPathToCompleteConfigurationFile, std::ios::app);
  if (!configuration_file.is_open()) {
    std::cout << turing_machine_.GetConfigurationForConsole() << std::endl;
  } else {
    // NOTE: the 2 spaces are necessary to start a newline for the next complete
    // configuration in the markdown file
    configuration_file << turing_machine_.GetConfigurationForMarkdown() << "  " 
        << '\n';
  }
  
  simulation_is_in_progress_ = false;
  // resume normal frame rate from reduced frame rate so graphics aren't slow
  ci::app::setFrameRate(60);
  // the user will not likely want to continue editing these
  // when the simulation ends
  editing_tape_character_ = false;
  editing_state_name_ = false;
  index_of_add_arrow_text_to_edit = add_arrow_inputs_.size();
}

void TuringMachineSimulatorApp::DrawAddArrowMenu() const {
  // draw the add arrow box
  ci::gl::color(ci::Color("papayawhip"));
  const glm::vec2 kUpperCorner= glm::vec2(kAddArrowBoxXBoundary,
      kAddArrowBoxYBoundary);
  const glm::vec2 kLowerCorner = glm::vec2(kHorizontalWindowSize,
      kVerticalWindowSize);
  ci::gl::drawSolidRect(ci::Rectf(kUpperCorner, kLowerCorner));
  
  //outline the box
  ci::gl::color(ci::Color("black"));
  ci::gl::drawStrokedRect(ci::Rectf(kUpperCorner, kLowerCorner));
  
  // draw menu heading
  const float kXLocationOfHeading = ((kHorizontalWindowSize 
      - kAddArrowBoxXBoundary) / 2) + kAddArrowBoxXBoundary;
  // note: 5 was found by experimenting for best visual experience
  const glm::vec2 kLocationOfHeading = glm::vec2(kXLocationOfHeading, 
      kAddArrowBoxYBoundary + 5);
  ci::gl::drawStringCentered("ADD ARROW", kLocationOfHeading, 
      ci::Color("black"));
  
  // add input for read character;
  // note: 20 was found by experimenting for best visual experience
  const glm::vec2 kLocationOfRead = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 20);
  ci::gl::drawString("Read: ", kLocationOfRead, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kReadInputBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfReadInput], 
      kUpperCornerOfReadInput, ci::Color("black"));
  
  // add input for write character
  // note: 45 was found by experimenting for best visual experience
  const glm::vec2 kLocationOfWrite = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 45);
  ci::gl::drawString("Write: ", kLocationOfWrite, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kWriteInputBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfWriteInput],
      kUpperCornerOfWriteInput, ci::Color("black"));
  
  // add input for shift character
  // note: 70 was found by experimenting for best visual experience
  const glm::vec2 kLocationOfShift = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 70);
  ci::gl::drawString("Shift: ", kLocationOfShift, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kShiftInputBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfShiftInput], 
      kUpperCornerOfShiftInput, ci::Color("black"));
  
  // add input for move from
  // note: 95 was found by experimenting for best visual experience
  const glm::vec2 kLocationOfMoveFrom = glm::vec2(kAddArrowBoxXBoundary, 
      kAddArrowBoxYBoundary + 95);
  ci::gl::drawString("Move From: ", kLocationOfMoveFrom, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kMoveFromInputBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfMoveFromInput], 
      kUpperCornerOfMoveFromInput, ci::Color("black"));
  
  // add input for move to
  // note: 120 was found by experimenting for best visual experience
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
  // note: 140 was found by experimenting for best visual experience
  const glm::vec2 kLocationOfSubmit = glm::vec2(kXLocationOfHeading, 
      kAddArrowBoxYBoundary + 140);
  ci::gl::drawStringCentered("SUBMIT", kLocationOfSubmit, 
      ci::Color("black"));
}

void TuringMachineSimulatorApp::DrawArrow(const Direction &direction) const {
  glm::vec2 move_from_state_center;
  glm::vec2 move_to_state_center;

  // find the location of the states that the arrow is between
  for (const State &kState : states_) {
    // NOTE: not in an else-if statement because a direction may keep the same 
    // state, in which case the move from and move to states would be the same
    if (kState.Equals(direction.GetStateToMoveFrom())) {
      move_from_state_center = kState.GetStateLocation();
    }
    if (kState.Equals(direction.GetStateToMoveTo())) {
      move_to_state_center = kState.GetStateLocation();
    }
  }
  // draw a line between the 2 states
  ci::gl::color(ci::Color("black"));
  ci::gl::drawLine(move_from_state_center, move_to_state_center);

  // display the direction instructions besides the line
  const glm::vec2 kLocationOfDirectionText =
      turingmachinesimulator::TuringMachineSimulatorHelper
      ::GetDirectionTextLocation(move_from_state_center, move_to_state_center,
      kRadiusOfStates);
  ci::gl::drawString(direction.ToString(), kLocationOfDirectionText,
      "black");

  // draw the arrow onto the line
  const std::tuple<glm::vec2, glm::vec2, glm::vec2> kArrow =
      turingmachinesimulator::TuringMachineSimulatorHelper::GetArrow
      (move_from_state_center, move_to_state_center);
  ci::gl::color(ci::Color("black"));
  const size_t kIndexOfFirstPoint = 0;
  const size_t kIndexOfArrowTip = 1;
  const size_t kIndexOfThirdPoint = 2;
  ci::gl::drawLine(std::get<kIndexOfFirstPoint>(kArrow),
      std::get<kIndexOfArrowTip>(kArrow));
  ci::gl::drawLine(std::get<kIndexOfArrowTip>(kArrow),
      std::get<kIndexOfThirdPoint>(kArrow));
}

void TuringMachineSimulatorApp::DrawTape() const {
  const double kPixelLengthOfTape = kLowerCornerOfTape.x - kUpperCornerOfTape.x;
  const double kHorizontalSizeOfSquares = kPixelLengthOfTape / tape_.size();
  glm::vec2 square_upper_corner = kUpperCornerOfTape;
  glm::vec2 square_lower_corner = glm::vec2(kUpperCornerOfTape.x
     + kHorizontalSizeOfSquares, kLowerCornerOfTape.y);

  ci::gl::color(ci::Color("black"));
  for (size_t i = 0; i < tape_.size(); i++) {
    // draw the square
    const ci::Rectf kSquare = ci::Rectf(square_upper_corner, square_lower_corner);
    ci::gl::drawStrokedRect(kSquare);

    // display the character in the square
    const glm::vec2 kCenterOfSquare =  turingmachinesimulator
    ::TuringMachineSimulatorHelper::GetCenterOfRectangle(kSquare);
    std::stringstream character_as_stringstream;
    character_as_stringstream << tape_.at(i);
    ci::gl::drawString(character_as_stringstream.str(), kCenterOfSquare,
        "black");

    // draw the scanner if it is reading this character
    if (i == index_of_character_being_read_) {
      const int kVerticalDistanceFromTape = 100;
      const glm::vec2 kLowerLeftPoint = glm::vec2(square_upper_corner.x,
          square_lower_corner.y + kVerticalDistanceFromTape);
      const glm::vec2 kLowerRightPoint = glm::vec2(square_lower_corner.x,
          square_lower_corner.y + kVerticalDistanceFromTape);
      const glm::vec2 kPointOfTriangle = glm::vec2(kCenterOfSquare.x,
          square_lower_corner.y);
      ci::gl::drawSolidTriangle(kLowerLeftPoint, kPointOfTriangle,
          kLowerRightPoint);
    }
    // increment the horizontal coordinates of the square
    square_upper_corner.x = square_lower_corner.x;
    square_lower_corner.x += kHorizontalSizeOfSquares;
  }
}

void TuringMachineSimulatorApp::DisplayErrorMessage() const {
  // if the turing machine is not empty, there is no error message to display
  if (turing_machine_.IsEmpty()) {
    const std::string kTuringMachineError = turing_machine_.GetErrorMessage();
    // if the error message is empty, there is no error message to display
    if (!kTuringMachineError.empty()) {
      const std::string kErrorMessage = "ERROR: "
          + turing_machine_.GetErrorMessage();
      // note: 10 was found by experimenting for best visual experience
      const glm::vec2 kPositionOfErrorMessage = glm::vec2(kMenuXBoundary / 2,
          10);
      ci::gl::drawStringCentered(kErrorMessage, kPositionOfErrorMessage,
          "firebrick");
    }
  }
}

void TuringMachineSimulatorApp::DrawButton(const ci::Rectf &button,
    const std::string &label, const ci::Color &color) const {
  ci::gl::color(color);
  ci::gl::drawSolidRect(button);
  ci::gl::color(ci::Color("black"));
  ci::gl::drawStrokedRect(button);
  ci::gl::drawStringCentered(label, turingmachinesimulator
      ::TuringMachineSimulatorHelper::GetCenterOfRectangle(button), 
      "black");
}

} // namespace turingmachinesimulator
