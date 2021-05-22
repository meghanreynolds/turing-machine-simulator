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
  
  // display settings page if applicable
  if (settings_is_showing_) {
    DisplaySettingsPage();
    return;
  }
  
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
  DrawButton(kClearButton, "CLEAR", ci::Color("salmon"));
  
  //display the simulate button
  DrawButton(kSimulateButton, "SIMULATE!",
      ci::Color("mediumspringgreen"));
  
  // display the tape
  DrawTape();
  
  // display reset tape button
  DrawButton(kResetTapeButton, "Reset Tape",
      ci::Color("salmon"));
  
  // display settings button
  DrawButton(kSettingsButton, "Settings",
      ci::Color("papayawhip"));
  
  // display stop simulation button if simulation is in progress and the step 
  // through button if in step-through mode
  if (simulation_is_in_progress_) {
    DrawButton(kStopSimulationButton, "STOP!",
        ci::Color("salmon"));
    if (in_step_through_mode_) {
      DrawButton(kStepThroughButton, "STEP!", 
          ci::Color("mediumspringgreen"));
    }
  }
  
  // display user-defined directions
  // NOTE: this must come before displaying user-defined states so the arrow 
  // goes behind the states
  for (const Direction &kDirection : directions_) {
    DrawArrow(kDirection);
  }

  // display user-defined states and highlight the current state of the 
  // simulation (if simulation is in progress)
  for (const State &kState : states_) {
    // note: 10 was found to be the best size of the highlight ring by 
    // experimenting for best visual experience
    const int kSizeOfHighlightRing = 10;
    if (simulation_is_in_progress_ 
        && turing_machine_.GetCurrentState().Equals(kState)) {
      ci::gl::color(ci::Color("yellow"));
      ci::gl::drawSolidCircle(kState.GetStateLocation(), 
          kState.GetRadius() + kSizeOfHighlightRing);
      
    } else if (halting_state_to_highlight_.Equals(kState) 
        && !halting_state_to_highlight_.IsEmpty()) {
      ci::gl::color(ci::Color("yellow"));
      ci::gl::drawSolidCircle(kState.GetStateLocation(),
          kState.GetRadius() + kSizeOfHighlightRing);
      num_times_halting_state_highlighted_ += 1;
      // note: 45 was found to be the best number of iterations for the halting
      // state to be highlighted by experimenting for best visual experience
      const int kMaxNumTimesHaltingStateHighlighted = 45;
      if (num_times_halting_state_highlighted_ 
          > kMaxNumTimesHaltingStateHighlighted) {
        halting_state_to_highlight_ = State();
        num_times_halting_state_highlighted_ = 0;
      }
    }
    kState.Display();
  }

  // display the error message
  // NOTE: This is displayed last so that nothing on the screen covers it up
  DisplayErrorMessage();
}

void TuringMachineSimulatorApp::update() {
  // only use the update function while the simulation is in-progress and not
  // in step-through mode
  if(simulation_is_in_progress_ && !in_step_through_mode_) {
    // significantly reduce frame rate (1 FPS) to allow users to really see how 
    // the scanner is moving
    ci::app::setFrameRate(1);
    // this forces the first state to be highlighted before starting the 
    // simulation
    if (is_first_turn_of_simulation_) {
      is_first_turn_of_simulation_ = false;
      return;
    }
    PerformTuringMachineStep();
  }
}

void TuringMachineSimulatorApp::mouseDown(ci::app::MouseEvent event) {
  const glm::vec2 kClickLocation = event.getPos();

  if (simulation_is_in_progress_) {
    HandleSimulationButtons(kClickLocation);
    return;
  }
  
  if (settings_is_showing_) {
    HandleSettingsButtons(kClickLocation);
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
  // NOTE: this is necessary for in case a user clicks a state and then clicks 
  // the canvas
  clicked_state_ = State(); 
}

void TuringMachineSimulatorApp::mouseDrag(ci::app::MouseEvent event) {
  // if clicked state is empty or the simulation is in progress or the settings
  // page is being displayed, there's nothing to drag
  if (clicked_state_.IsEmpty() || simulation_is_in_progress_ 
      || settings_is_showing_) {
    return;
  }
  // update the position of the clicked state to be where the user dragged it
  TuringMachineSimulatorHelper::UpdateStatePosition(clicked_state_, 
      states_, event.getPos(), halting_state_names_);
}

void TuringMachineSimulatorApp::keyDown(ci::app::KeyEvent event) {
  // if simulation is in progress, keyboard input is not supported
  if (simulation_is_in_progress_) {
    return;
  }
  
  const int kEventCode = event.getCode();
  // NOTE: shift keys must be explicitly handled in this UI
  if (kEventCode == ci::app::KeyEvent::KEY_LSHIFT || kEventCode 
      == ci::app::KeyEvent::KEY_RSHIFT) {
    return;
  }
  
  if (settings_is_showing_) {
    if (editing_blank_char_) {
      EditBlankCharacter(event);
    }
    return;
  }
  
  // editing tape character takes precedence to all other edits
  if (editing_tape_character_) {
    EditTapeCharacter(event);
    return;
  }
  
  // editing state name takes precedence to adding arrows
  if (editing_state_name_) {
    editing_state_name_ = TuringMachineSimulatorHelper::UpdateStateName(
        states_, directions_, state_being_modified_, event.getChar(),
        event.getCode());
    return;
  }
  
  // edit add arrow text if applicable
  if (index_of_add_arrow_text_to_edit != add_arrow_inputs_.size()) {
    index_of_add_arrow_text_to_edit = TuringMachineSimulatorHelper
        ::UpdateAddArrowInputs(add_arrow_inputs_, 
        index_of_add_arrow_text_to_edit, event.getChar(), event.getCode());
  }
}

void TuringMachineSimulatorApp::PerformTuringMachineStep() {
  // update the turing machine
  turing_machine_.Update();

  // add the current turing machine configuration to the console output or
  // the markdown file, depending on whether the markdown file exists or not
  std::ofstream configuration_file =
      std::ofstream(kPathToCompleteConfigurationFile, std::ios::app);
  if (!configuration_file.is_open()) {
    std::cout << turing_machine_.GetConfigurationForConsole();
  } else {
    configuration_file << turing_machine_.GetConfigurationForMarkdown();
  }

  // update the tape, scanner, and stop simulation if applicable
  tape_ = turing_machine_.GetTape();
  index_of_character_being_read_ = turing_machine_.GetIndexOfScanner();
  if (turing_machine_.IsHalted()) {
    halting_state_to_highlight_ = turing_machine_.GetCurrentState();
    // NOTE: Lines 259-263 must not be added to StopSimulation()
    if (!configuration_file.is_open()) {
      std::cout << '\n';
    } else {
      configuration_file << "  " << "\n";
    }
    StopSimulation();
  }
}

void TuringMachineSimulatorApp::HandleSimulationButtons(const glm::vec2 
    &click_location) {
  if (turingmachinesimulator::TuringMachineSimulatorHelper::IsPointInRectangle(
      click_location, kStopSimulationButton)) {
    // NOTE: Line 273 - 279 must not be added to StopSimulation()
    std::ofstream configuration_file = std::ofstream(
        kPathToCompleteConfigurationFile, std::ios::app);
    if (!configuration_file.is_open()) {
      std::cout << '\n';
    } else {
      configuration_file << "  " << "\n";
    }
    StopSimulation();
  }
  
  if (in_step_through_mode_) {
    if (turingmachinesimulator::TuringMachineSimulatorHelper::IsPointInRectangle(
        click_location, kStepThroughButton)) {
      PerformTuringMachineStep();
    }
  }
}

void TuringMachineSimulatorApp::HandleSettingsButtons(const glm::vec2 
    &click_location) {
  if (turingmachinesimulator::TuringMachineSimulatorHelper::
      IsPointInRectangle(click_location, kBlankCharInputBox)) {
    editing_blank_char_ = true;

  } else if ((turingmachinesimulator::TuringMachineSimulatorHelper::
      IsPointInRectangle(click_location, kExitSettingsButton))) {
    editing_blank_char_ = false;
    settings_is_showing_ = false;

  } else if ((turingmachinesimulator::TuringMachineSimulatorHelper
      ::IsPointInCircle(click_location, kStepThroughToggleOnCenter,
      kStepThroughToggleRadii))) {
    in_step_through_mode_ = true;

  } else if ((turingmachinesimulator::TuringMachineSimulatorHelper
      ::IsPointInCircle(click_location, kStepThroughToggleOffCenter,
      kStepThroughToggleRadii))) {
    in_step_through_mode_ = false;
  }
}

bool TuringMachineSimulatorApp::HandleClickedBox(const glm::vec2
    &click_location) {
  bool input_box_was_clicked = false;

  if (turingmachinesimulator::TuringMachineSimulatorHelper::IsPointInRectangle(
      click_location, kReadInputBox)) {
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
    turingmachinesimulator::TuringMachineSimulatorHelper::AddDirection(
        add_arrow_inputs_, directions_, states_);
    index_of_add_arrow_text_to_edit = add_arrow_inputs_.size();
    input_box_was_clicked = true;

  } else if (turingmachinesimulator::TuringMachineSimulatorHelper
      ::IsPointInRectangle(click_location, kClearButton)) {
    // reset all modifiable global variables when clear is clicked
    directions_ = {};
    states_ = {};
    turingmachinesimulator::TuringMachineSimulatorHelper::ResetTape(tape_,
        blank_character_);
    index_of_character_being_read_ = 0;
    simulation_is_in_progress_ = false;
    turing_machine_ = TuringMachine();
    is_first_turn_of_simulation_ = true;
    halting_state_to_highlight_ = State();
    num_times_halting_state_highlighted_ = 0;
    state_id_ = 0;
    add_arrow_inputs_ = {"single char", "single char", "L/R/N", "q5", "qh"};
    index_of_add_arrow_text_to_edit = add_arrow_inputs_.size();
    editing_state_name_ = false;
    state_being_modified_ = State();
    index_of_tape_character_being_edited_ = tape_.size();
    editing_tape_character_ = false;
    input_box_was_clicked = true;

  } else if (turingmachinesimulator::TuringMachineSimulatorHelper
      ::IsPointInRectangle(click_location, kSimulateButton)) {
    // create a turing machine from user-defined states, directions, and tape
    // on simulate button click
    turing_machine_ = TuringMachine(states_, directions_, tape_, blank_character_,
        halting_state_names_);
    // if turing machine is empty, the creation of a turing machine was 
    // unsuccessful so we want to display the error message. Otherwise, we
    // want to being the simulation
    if (turing_machine_.IsEmpty()) {
      DisplayErrorMessage();
    } else {
      StartSimulation();
    }
    input_box_was_clicked = true;

  } else if (turingmachinesimulator::TuringMachineSimulatorHelper
      ::IsPointInRectangle(click_location, kResetTapeButton)) {
    turingmachinesimulator::TuringMachineSimulatorHelper::ResetTape(tape_,
        blank_character_);
    index_of_character_being_read_ = 0;
    input_box_was_clicked = true;
    
  } else if (turingmachinesimulator::TuringMachineSimulatorHelper::
      IsPointInRectangle(click_location, kSettingsButton)) {
    settings_is_showing_ = true;
    input_box_was_clicked = true;
  }

  // if a square of the tape is clicked, then the user is about to edit
  // the square of the tape that they clicked on
  const size_t kIndexOfTapeSquareClicked = turingmachinesimulator
      ::TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked(
      click_location, tape_.size(), kUpperCornerOfTape, 
      kLowerCornerOfTape);
  if (kIndexOfTapeSquareClicked != tape_.size()) {
    editing_tape_character_ = true;
    index_of_tape_character_being_edited_ = kIndexOfTapeSquareClicked;
    input_box_was_clicked = true;
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
        kRadiusOfStates, halting_state_names_);
    new_state_was_created = true;
  }

  if (kExampleNthState.IsStateCenterWithinGivenRadiusOfPoint(click_location,
      kRadiusOfStates)) {
    const std::string kNthStateName = "qn";
    new_state = State(state_id_, kNthStateName, click_location, kRadiusOfStates, 
        halting_state_names_);
    new_state_was_created = true;
  }

  if (kExampleHaltingState.IsStateCenterWithinGivenRadiusOfPoint(click_location,
      kRadiusOfStates)) {
    const std::string kHaltingStateName = "qh";
    new_state = State(state_id_, kHaltingStateName, click_location,
        kRadiusOfStates, halting_state_names_);
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
      kDoubleTheRadii) || clicked_state_.IsStateCenterWithinGivenRadiusOfPoint(
      kLocationOfQn, kDoubleTheRadii) || 
      clicked_state_.IsStateCenterWithinGivenRadiusOfPoint(kLocationOfQh,
      kDoubleTheRadii)) {
    turingmachinesimulator::TuringMachineSimulatorHelper::DeleteGivenState(
        clicked_state_, states_, directions_);
    return true;
  }
  return false;
}

void TuringMachineSimulatorApp::EditBlankCharacter(const ci::app::KeyEvent 
    &key_event) {
  const std::tuple<char, bool> kUpdatedBlankCharResult =
      TuringMachineSimulatorHelper::UpdateBlankCharacter(key_event.getChar(),
      key_event.getCode());
  const size_t kIndexOfChar = 0;
  const size_t kIndexOfStillEditingBool = 1;
  if (!std::get<kIndexOfStillEditingBool>(kUpdatedBlankCharResult)) {
    editing_blank_char_ = false;
  } else {
    blank_character_ = std::get<kIndexOfChar>(kUpdatedBlankCharResult);
  }
  // reset the tape (and scanner) in order to update it with the new blank 
  // character
  turingmachinesimulator::TuringMachineSimulatorHelper::ResetTape(tape_,
      blank_character_);
  index_of_character_being_read_ = 0;
}

void TuringMachineSimulatorApp::EditTapeCharacter(const ci::app::KeyEvent
    &key_event) {
  index_of_tape_character_being_edited_ = TuringMachineSimulatorHelper
      ::UpdateTapeCharacter(tape_, blank_character_, 
      index_of_tape_character_being_edited_, key_event.getChar(),
      key_event.getCode());
  if (index_of_tape_character_being_edited_ == tape_.size()) {
    editing_tape_character_ = false;
  }
}

void TuringMachineSimulatorApp::StartSimulation() {
  num_simulations_run_ += 1;
  simulation_is_in_progress_ = true;
  
  // label the complete configuration with a heading indicating which
  // number simulation this is
  std::ofstream configuration_file = std::ofstream(
      kPathToCompleteConfigurationFile, std::ios::app);
  if (configuration_file.is_open()) {
    // NOTE: "##" is the markdown tag for a secondary heading
    configuration_file << "## Simulation " << num_simulations_run_
        << " Complete Configuration:  ##\n"
        << turing_machine_.GetConfigurationForMarkdown();
  } else {
    std::cout << "Simulation " << num_simulations_run_
        << " Complete Configuration: " << std::endl
        << turing_machine_.GetConfigurationForConsole();
  }
}

void TuringMachineSimulatorApp::StopSimulation() {
  simulation_is_in_progress_ = false;
  is_first_turn_of_simulation_ = true;
  // resume normal frame rate from reduced frame rate so graphics aren't slow
  ci::app::setFrameRate(60);
  // the user will not likely want to continue editing these
  // when the simulation ends (if they were when it began)
  editing_tape_character_ = false;
  editing_state_name_ = false;
  index_of_add_arrow_text_to_edit = add_arrow_inputs_.size();
}

void TuringMachineSimulatorApp::DisplaySettingsPage() const {
  // set background color
  ci::gl::color(ci::Color("papayawhip"));
  const glm::vec2 kUpperScreenCorner = glm::vec2(0, 0);
  const glm::vec2 kLowerScreenCorner = glm::vec2(kHorizontalWindowSize,
      kVerticalWindowSize);
  const ci::Rectf kWindow = ci::Rectf(kUpperScreenCorner, kLowerScreenCorner);
  ci::gl::drawSolidRect(kWindow);
  
  // display page name
  const double kTitleXLocation = kWindow.getCenter().x;
  // note: 10 was found after experimenting for best visual experience
  const glm::vec2 kTitleLocation = glm::vec2(kTitleXLocation, 10);
  ci::gl::drawString("SETTINGS", kTitleLocation, "black");
  
  // display exit button
  DrawButton(kExitSettingsButton, "EXIT",
      ci::Color("firebrick"));
  
  // display blank character option
  // note: 10 was found after experimenting for best visual experience
  const glm::vec2 kBlankCharLabelLocation = glm::vec2(10, 
      kBlankCharInputBox.getCenter().y);
  ci::gl::drawString("Blank Character: ", kBlankCharLabelLocation, 
      "black");
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kBlankCharInputBox);
  std::stringstream blank_char_as_stringstream;
  blank_char_as_stringstream << blank_character_;
  ci::gl::drawString(blank_char_as_stringstream.str(), 
      kBlankCharInputBox.getCenter(), "black");
  
  // display step through toggle
  // note: 10 was found after experimenting for best visual experience
  const glm::vec2 kStepThroughLabelLocation = glm::vec2(10,
      kStepThroughToggleOnCenter.y);
  ci::gl::drawString("Step Through: ", kStepThroughLabelLocation,
      "black");
  // toggle on is only green when in step through mode
  if (in_step_through_mode_) {
    ci::gl::color(ci::Color("mediumspringgreen"));
  } else {
    ci::gl::color(ci::Color("white"));
  }
  ci::gl::drawSolidCircle(kStepThroughToggleOnCenter, kStepThroughToggleRadii);
  ci::gl::drawStringCentered("on", kStepThroughToggleOnCenter, 
      "black");
  // toggle off is only green when not in step through mode
  if (in_step_through_mode_) {
    ci::gl::color(ci::Color("white"));
  } else {
    ci::gl::color(ci::Color("mediumspringgreen"));
  }
  ci::gl::drawSolidCircle(kStepThroughToggleOffCenter, kStepThroughToggleRadii);
  ci::gl::drawStringCentered("off", kStepThroughToggleOffCenter,
      "black");
}

void TuringMachineSimulatorApp::DrawAddArrowMenu() const {
  // draw the add arrow box
  ci::gl::color(ci::Color("papayawhip"));
  const glm::vec2 kUpperCorner= glm::vec2(kAddArrowBoxXBoundary,
      kAddArrowBoxYBoundary);
  const glm::vec2 kLowerCorner = glm::vec2(kHorizontalWindowSize,
      kVerticalWindowSize);
  const ci::Rectf kAddArrowBox = ci::Rectf(kUpperCorner, kLowerCorner);
  ci::gl::drawSolidRect(kAddArrowBox);
  
  //outline the box
  ci::gl::color(ci::Color("black"));
  ci::gl::drawStrokedRect(kAddArrowBox);
  
  // draw box heading
  const float kXLocationOfHeading = kAddArrowBox.getCenter().x;
  // note: 5 was found by experimenting for best visual experience
  const glm::vec2 kLocationOfHeading = glm::vec2(kXLocationOfHeading, 
      kAddArrowBoxYBoundary + 5);
  ci::gl::drawStringCentered("ADD ARROW", kLocationOfHeading, 
      ci::Color("black"));
  
  // add input for read character;
  const double kVerticalCenterOfReadBox = kReadInputBox.getCenter().y;
  const glm::vec2 kLocationOfRead = glm::vec2(kAddArrowBoxXBoundary, 
      kVerticalCenterOfReadBox);
  ci::gl::drawString("Read: ", kLocationOfRead,
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kReadInputBox);
  const glm::vec2 kReadInputTextLocation = glm::vec2(kUpperCornerOfReadInput.x, 
      kVerticalCenterOfReadBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfReadInput], 
      kReadInputTextLocation, ci::Color("black"));
  
  // add input for write character
  const double kVerticalCenterOfWriteBox = kWriteInputBox.getCenter().y;
  const glm::vec2 kLocationOfWrite = glm::vec2(kAddArrowBoxXBoundary, 
      kVerticalCenterOfWriteBox);
  ci::gl::drawString("Write: ", kLocationOfWrite, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kWriteInputBox);
  const glm::vec2 kWriteInputTextLocation = glm::vec2(kUpperCornerOfWriteInput.x, 
      kVerticalCenterOfWriteBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfWriteInput],
      kWriteInputTextLocation, ci::Color("black"));
  
  // add input for shift character
  const double kVerticalCenterOfShiftBox = kShiftInputBox.getCenter().y;
  const glm::vec2 kLocationOfShift = glm::vec2(kAddArrowBoxXBoundary, 
      kVerticalCenterOfShiftBox);
  ci::gl::drawString("Shift: ", kLocationOfShift, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kShiftInputBox);
  const glm::vec2 kShiftInputTextLocation = glm::vec2(kUpperCornerOfShiftInput.x, 
      kVerticalCenterOfShiftBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfShiftInput], 
      kShiftInputTextLocation, ci::Color("black"));
  
  // add input for move from state name
  const double kVerticalCenterOfMoveFromBox = kMoveFromInputBox.getCenter().y;
  const glm::vec2 kLocationOfMoveFrom = glm::vec2(kAddArrowBoxXBoundary, 
      kVerticalCenterOfMoveFromBox);
  ci::gl::drawString("Move From: ", kLocationOfMoveFrom, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kMoveFromInputBox);
  const glm::vec2 kMoveFromInputTextLocation = glm::vec2(
      kUpperCornerOfMoveFromInput.x, kVerticalCenterOfMoveFromBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfMoveFromInput], 
      kMoveFromInputTextLocation, ci::Color("black"));
  
  // add input for move to state name
  const double kVerticalCenterOfMoveToBox = kMoveToInputBox.getCenter().y;
  const glm::vec2 kLocationOfMoveTo = glm::vec2(kAddArrowBoxXBoundary, 
      kVerticalCenterOfMoveToBox);
  ci::gl::drawString("Move To: ", kLocationOfMoveTo, 
      ci::Color("black"));
  ci::gl::color(ci::Color("white"));
  ci::gl::drawSolidRect(kMoveToInputBox);
  const glm::vec2 kMoveToInputTextLocation = glm::vec2(
      kUpperCornerOfMoveToInput.x, kVerticalCenterOfMoveToBox);
  ci::gl::drawString(add_arrow_inputs_[kIndexOfMoveToInput], 
      kMoveToInputTextLocation, ci::Color("black"));
  
  //add submit button
  DrawButton(kSubmitButton, "SUBMIT", 
      ci::Color("honeydew"));
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
  const glm::vec2 kLocationOfDirectionText = turingmachinesimulator
      ::TuringMachineSimulatorHelper::GetDirectionTextLocation(
      move_from_state_center, move_to_state_center, kRadiusOfStates);
  ci::gl::drawString(direction.ToString(), kLocationOfDirectionText,
      "black");

  // draw the arrow onto the line
  const std::tuple<glm::vec2, glm::vec2, glm::vec2> kArrow =
      turingmachinesimulator::TuringMachineSimulatorHelper::GetArrow(
      move_from_state_center, move_to_state_center);
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
  // note: cannot be in for-each loop because the index is needed
  for (size_t i = 0; i < tape_.size(); i++) {
    // draw the square
    const ci::Rectf kSquare = ci::Rectf(square_upper_corner, square_lower_corner);
    ci::gl::drawStrokedRect(kSquare);

    // display the character in the square
    const glm::vec2 kCenterOfSquare =  kSquare.getCenter();
    std::stringstream character_as_stringstream;
    character_as_stringstream << tape_.at(i);
    ci::gl::drawString(character_as_stringstream.str(), kCenterOfSquare,
        "black");

    // draw the scanner if it is reading this character
    if (i == index_of_character_being_read_) {
      // note: 100 was found to be the best distance from the tape after 
      // experimentation for the best visual effect
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
      // note: division by 2 to be located at center of the canvas (screen not
      // including the menu)
      const glm::vec2 kPositionOfErrorMessage = glm::vec2((kMenuXBoundary 
          / 2), 10);
      ci::gl::drawStringCentered(kErrorMessage, kPositionOfErrorMessage,
          "firebrick");
    }
  }
}

void TuringMachineSimulatorApp::DrawButton(const ci::Rectf &button,
    const std::string &label, const ci::Color &color) const {
  // fill the button with the given color
  ci::gl::color(color);
  ci::gl::drawSolidRect(button);
  // outline button
  ci::gl::color(ci::Color("black"));
  ci::gl::drawStrokedRect(button);
  // add button label
  ci::gl::drawStringCentered(label, button.getCenter(), "black");
}

} // namespace turingmachinesimulator
