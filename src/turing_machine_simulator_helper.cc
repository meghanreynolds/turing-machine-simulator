#include "turing_machine_simulator_helper.h"

namespace turingmachinesimulator {

bool TuringMachineSimulatorHelper::IsPointInRectangle(const glm::vec2 
    &point, const ci::Rectf &rectangle) {
  const glm::vec2 kRectangleUpperLeftCorner = rectangle.getUpperLeft();
  const glm::vec2 kRectangleLowerRightCorner = rectangle.getLowerRight();
  if (point.x >= kRectangleUpperLeftCorner.x && point.x
      <= kRectangleLowerRightCorner.x) {
    if (point.y >= kRectangleUpperLeftCorner.y && point.y <=
        kRectangleLowerRightCorner.y) {
      return true;
    }
  }
  return false;
}

glm::vec2 TuringMachineSimulatorHelper::GetCenterOfRectangle(const ci::Rectf 
    &rectangle) {
  // the variables below use division by 2 to get the midpoint
  const double kHorizontalCenterOfRectangle = (rectangle.getUpperLeft().x 
      + rectangle.getLowerRight().x) / 2;
  const double kVerticalCenterOfRectangle = (rectangle.getUpperLeft().y
      + rectangle.getLowerRight().y) / 2;
  return glm::vec2(kHorizontalCenterOfRectangle, kVerticalCenterOfRectangle);
}

glm::vec2 TuringMachineSimulatorHelper::GetDirectionTextLocation(const glm::vec2 
    &point_a, const glm::vec2 &point_b, double state_radius) {
  const int kSpaceFromLine = 10;
  if (point_b == point_a) {
    return glm::vec2(point_a.x, point_a.y - state_radius - kSpaceFromLine);
  }
  const double kDistanceForOverlappingStates = state_radius * 2;
  if (glm::distance(point_a, point_b) <= kDistanceForOverlappingStates) {
    return glm::vec2(point_a.x, point_a.y - state_radius - kSpaceFromLine);
  }
  // divide by 2 to get midpoint
  const double kHorizontalMidpoint = (point_a.x + point_b.x) / 2; 
  const double kVerticalMidpoint = (point_a.y + point_b.y) / 2;
  return glm::vec2(kHorizontalMidpoint, kVerticalMidpoint - kSpaceFromLine);
}

size_t TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked(const 
    glm::vec2 &clicked_point, size_t tape_length, const glm::vec2 
    &tape_upper_corner, const glm::vec2 &tape_lower_corner) {
  // the size of the tape is returned when no squares were found to have been
  // clicked
  if (!IsPointInRectangle(clicked_point, ci::Rectf(tape_upper_corner,
      tape_lower_corner))) {
    return tape_length; 
  }
  const double kTapePixelLength = tape_lower_corner.x - tape_upper_corner.x;
  const double kSquareHorizontalSize = kTapePixelLength / tape_length;
  glm::vec2 square_upper_corner = tape_upper_corner;
  glm::vec2 square_lower_corner = glm::vec2(tape_upper_corner.x
      + kSquareHorizontalSize, tape_lower_corner.y);
  size_t index_of_square = 0;
  while (index_of_square < tape_length) {
    const ci::Rectf kSquare = ci::Rectf(square_upper_corner, square_lower_corner);
    if (IsPointInRectangle(clicked_point, kSquare)) {
      return index_of_square;
    }
    index_of_square += 1;
    square_upper_corner.x = square_lower_corner.x;
    square_lower_corner.x = square_lower_corner.x + kSquareHorizontalSize;
  }
  return tape_length;
}

std::tuple<glm::vec2, glm::vec2, glm::vec2> TuringMachineSimulatorHelper
    ::GetArrow(const glm::vec2 &point_a, const glm::vec2 &point_b) {
  // the tip of the arrow is located at 3/4 of the line between the 2 points
  const double kHorizontalPoint = (point_a.x + (3 * point_b.x)) / 4;
  const double kVerticalPoint = (point_a.y + (3 * point_b.y)) / 4;
  const glm::vec2 kTipOfArrow = glm::vec2(kHorizontalPoint, 
      kVerticalPoint);
  
  // Lines 81-97 adapted from:
  // http://kapo-cpp.blogspot.com/2008/10/drawing-arrows-with-cairo.html
  // note: 10 and 20 were found by experimenting for best visual experience
  const double kArrowLineLength = 10;
  const double kArrowInternalAngle = 20;
  const double kArrowAngle = std::atan2(point_b.y - point_a.y, 
      point_b.x - point_a.x) + M_PI;
  
  const double kFirstPointXValue = kTipOfArrow.x + kArrowLineLength
      * std::cos(kArrowAngle - kArrowInternalAngle);
  const double kFirstPointYValue = kTipOfArrow.y + kArrowLineLength 
      * std::sin(kArrowAngle - kArrowInternalAngle);
  const glm::vec2 kFirstPoint = glm::vec2(kFirstPointXValue, kFirstPointYValue);
  
  const double kThirdPointXValue = kTipOfArrow.x + kArrowLineLength 
      * std::cos(kArrowAngle + kArrowInternalAngle);
  const double kThirdPointYValue = kTipOfArrow.y + kArrowLineLength 
      * std::sin(kArrowAngle + kArrowInternalAngle);
  const glm::vec2 kThirdPoint = glm::vec2(kThirdPointXValue, 
      kThirdPointYValue);
  
  return std::tuple<glm::vec2, glm::vec2, glm::vec2>(kFirstPoint, kTipOfArrow,
      kThirdPoint);
}

void TuringMachineSimulatorHelper::AddDirection(const 
    std::vector<std::string> &add_direction_inputs, std::vector<Direction>
    &directions, const std::vector<State> &states) {
  // create a direction from the inputted information
  Direction direction = Direction(add_direction_inputs, states);
  
  // if direction is empty, the creation of a direction was unsuccessful, so
  // we don't want to add it to the list of directions
  if (!direction.IsEmpty()) {
    directions.push_back(direction);
  }
}

void TuringMachineSimulatorHelper::ResetTape(std::vector<char> &tape, 
    char blank_character) {
  tape = {blank_character, blank_character, blank_character, blank_character,
          blank_character, blank_character, blank_character, blank_character};
}

void TuringMachineSimulatorHelper::DeleteGivenState(const State &state_to_delete,
    std::vector<State> &states, std::vector<Direction> &directions) {
  // iterate through the states and remove the clicked_state_ node from the
  // list of active states
  State state_to_erase;
  // need to use for-each loop here because we need the index of the state
  // to delete
  for (size_t i = 0; i < states.size(); i++) {
    State current_state = states.at(i);
    if (current_state.Equals(state_to_delete)) {
      state_to_erase = current_state;
      states.erase(states.begin() + i);
      break; // once we've found the state to erase, nothing to search for
    }
  }

  // iterate through the directions and remove all directions involving
  // the deleted state
  const std::vector<Direction> kOriginalDirections = directions;
  size_t index_in_directions = 0;
  for (const Direction &kDirection : kOriginalDirections) {
    // NOTE: Must be in an else if statement in case state to move to is the
    // same as the state to move from
    if (kDirection.GetStateToMoveTo().Equals(state_to_erase)) {
      directions.erase(directions.begin() + index_in_directions);
      index_in_directions -= 1;
    } else if (kDirection.GetStateToMoveFrom().Equals(state_to_erase)) {
      directions.erase(directions.begin() + index_in_directions);
      index_in_directions -= 1;
    }
    index_in_directions += 1;
  }
}

void TuringMachineSimulatorHelper::UpdateStatePosition(State &clicked_state,
    std::vector<State> &states, const glm::vec2 &click_location, const 
    std::vector<std::string> &halting_state_names) {
  // cannot do a for-each loop here because we need the index in states_
  for (size_t i = 0; i < states.size(); i++) {
    const State kCurrentState = states.at(i);
    // update the clicked state's position to be where the user dragged it
    if (kCurrentState.Equals(clicked_state)) {
      const State kUpdatedState = State(kCurrentState.GetId(), 
          kCurrentState.GetStateName(), click_location, 
          kCurrentState.GetRadius(), halting_state_names);
      states[i] = kUpdatedState;
      clicked_state = states[i];
      break; // once the state to update has been found, nothing to search for
    }
  }
}

int TuringMachineSimulatorHelper::UpdateTapeCharacter(std::vector<char> &tape,
    char blank_character, int index_of_character_to_edit, char typed_char, 
    int event_code) {
  // on return, stop editing the tape character
  if (event_code == ci::app::KeyEvent::KEY_RETURN) {
    return tape.size();
  }

  // make the character a blank character on backspace
  if (event_code == ci::app::KeyEvent::KEY_BACKSPACE) {
    tape[index_of_character_to_edit] = blank_character;
    return index_of_character_to_edit;
  }

  // if no special event codes were typed, change the character to be the
  // character entered by the user
  tape[index_of_character_to_edit] = typed_char;
  return index_of_character_to_edit;
}

std::tuple<char, bool> TuringMachineSimulatorHelper
    ::UpdateBlankCharacter(char type_char, int event_code) {
  // on return, stop editing the blank character
  if (event_code == ci::app::KeyEvent::KEY_RETURN) {
    // NOTE: the character returned here does not matter as it will not be used
    return std::tuple<char, bool>('~', false);
  }

  // make the character a '-' character (default blank) on backspace
  if (event_code == ci::app::KeyEvent::KEY_BACKSPACE) {
    const char kDefaultBlank = '-';
    return std::tuple<char, bool>(kDefaultBlank, true);
  }

  // if no special event codes were typed, change the character to be the
  // character entered by the user
  return std::tuple<char, bool>(type_char, true);
}

bool TuringMachineSimulatorHelper::UpdateStateName(std::vector<State> &states,
    std::vector<Direction> &directions, State &state_being_modified, 
    char typed_char, int event_code) {
  // on return, stop editing the state name
  if (event_code == ci::app::KeyEvent::KEY_RETURN) {
    state_being_modified = State();
    return false;
  }

  // find the index in states_ of the state whose name is being modified
  size_t kIndexOfStateBeingModified;
  for (size_t i = 0; i < states.size(); i++) {
    const State kCurrentState = states.at(i);
    if (kCurrentState.Equals(state_being_modified)) {
      kIndexOfStateBeingModified = i;
      break; // break because there's nothing left to search for
    }
  }
  const std::string kNameToEdit = state_being_modified.GetStateName();

  // remove the last character from the string on backspace
  if (event_code == ci::app::KeyEvent::KEY_BACKSPACE) {
    // state name may not be less than 1 character (the q may not be deleted)
    if (kNameToEdit.size() > 1) {
      const size_t kLastCharacterOfName = kNameToEdit.size() - 1;
      const std::string kUpdatedName = kNameToEdit.substr(0,
          kLastCharacterOfName);
      state_being_modified.SetStateName(kUpdatedName);
      states[kIndexOfStateBeingModified] = state_being_modified;
      UpdateDirections(state_being_modified, directions);
    }
    return true;
  }

  // if no special event codes were typed, add the character that was typed
  // to the end of the state's name
  state_being_modified.SetStateName(kNameToEdit + typed_char);
  states[kIndexOfStateBeingModified] = state_being_modified;
  UpdateDirections(state_being_modified, directions);
  return true;
}

int TuringMachineSimulatorHelper::UpdateAddArrowInputs(std::vector<std::string> 
    &add_arrow_inputs, int index_of_input_to_edit, char typed_char, 
    int event_code) {
  // on return, stop editing the text that's being edited
  if (event_code == ci::app::KeyEvent::KEY_RETURN) {
    return add_arrow_inputs.size();
  }

  const std::string kTextToEdit = add_arrow_inputs[index_of_input_to_edit];
  // if backspace, remove 1 character from the end of the text being edited
  if (event_code == ci::app::KeyEvent::KEY_BACKSPACE) {
    // if size of text is less than or equal to 0, there's nothing to remove
    if (!kTextToEdit.empty()) {
      const size_t kLastCharacterOfText = kTextToEdit.size() - 1;
      add_arrow_inputs[index_of_input_to_edit] = kTextToEdit.substr(0, 
          kLastCharacterOfText);
    }
    return index_of_input_to_edit;
  }

  // if no special event codes were typed, then append the entered character to 
  // the text that's being edited
  add_arrow_inputs[index_of_input_to_edit] = kTextToEdit + typed_char;
  return index_of_input_to_edit;
}

void TuringMachineSimulatorHelper::UpdateDirections(const State &state,
    std::vector<Direction> &directions) {
  // NOTE: Cannot be for-each loop because index is necessary
  for (size_t i = 0; i < directions.size(); i++) {
    Direction direction = directions.at(i);
    // NOTE: Cannot be if-else statement because move from/to state can be the
    // same state
    if (direction.GetStateToMoveFrom().Equals(state)) {
      direction.SetStateToMoveFrom(state);
      directions[i] = direction;
    }
    if (direction.GetStateToMoveTo().Equals(state)) {
      direction.SetStateToMoveTo(state);
      directions[i] = direction;
    }
  }
}

} // namespace turingmachinesimulator
