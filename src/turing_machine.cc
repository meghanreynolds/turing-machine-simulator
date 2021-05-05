#include "turing_machine.h"

namespace turingmachinesimulator {

TuringMachine::TuringMachine(const std::vector<State> &states, const 
    std::vector<Direction> &directions, const std::vector<char> &tape, char 
    blank_character, const std::vector<std::string> &halting_state_names) {
  // if the given tape is empty, set it to 1 blank character (same thing as 
  // an empty tape)
  if (tape.empty()) {
    tape_ = {blank_character};
  } else {
    tape_ = tape;
  }
  
  // set starting and halting states
  for (const State &kState : states) {
    const std::string kNameOfStartingState = "q1";
    const std::string kStateName = kState.GetStateName();
    if (kStateName == kNameOfStartingState) {
      if (!current_state_.IsEmpty()) {
        error_message_ = "Cannot Have More Than 1 Starting State";
        return;
      } else {
        // the machine starts in the starting state
        current_state_ = kState;
      }
    } else if (std::find(halting_state_names.begin(), halting_state_names.end(),
        kStateName) != halting_state_names.end()) {
      halting_states_.push_back(kState);
    }
  }
  if (current_state_.IsEmpty()) {
    error_message_ = "Must Have Starting State";
    return;
  }
  
  // put directions into the directions by state map
 for (const Direction &kDirection : directions) {
    const State kStateToMoveFrom = kDirection.GetStateToMoveFrom();
    try {
      std::vector<Direction> kStateDirections = 
          directions_by_state_map_.at(kStateToMoveFrom);
      if (std::find(kStateDirections.begin(), kStateDirections.end(), kDirection)
          != kStateDirections.end()) {
        error_message_ = "Must Not Have 2 Directions With Same Read Condition "
            "From The Same State";
        return;
      }
    } catch (const std::exception &kException) {
      // Only here if the state to move from isn't in the map yet
      // Don't need to do anything since the above code would not apply
      // to such a situation anyway
    }
    directions_by_state_map_[kStateToMoveFrom].push_back(kDirection);
  }
 
  // if no errors were encountered in initializing the turing machine, then it is
  // not empty and initialize the blank character and halting state names
  is_empty_ = false;
  blank_character_ = blank_character;
  halting_state_names_ = halting_state_names;
}

State TuringMachine::GetCurrentState() const {
  return current_state_;
}

std::vector<State> TuringMachine::GetHaltingStates() const {
  return halting_states_;
}

std::map<State, std::vector<Direction>> TuringMachine::GetDirectionsByStateMap()
    const {
  return directions_by_state_map_;
}

std::vector<char> TuringMachine::GetTape() const {
  return tape_;
}

size_t TuringMachine::GetIndexOfScanner() const {
  return index_of_scanner_;
}

std::string TuringMachine::GetErrorMessage() const {
  return error_message_;
}

bool TuringMachine::IsHalted() const {
  return is_halted_;
}

bool TuringMachine::IsEmpty() const {
  return is_empty_;
}

std::string TuringMachine::GetConfigurationForConsole() const {
  std::stringstream configuration_stringstream;
  configuration_stringstream << ';';
  // cannot use for-each loop here since the index is necessary
  for (size_t i = 0; i < tape_.size(); i++) {
    if (i == index_of_scanner_) {
      configuration_stringstream << current_state_.GetStateName();
    }
    configuration_stringstream << tape_.at(i);
  }
  return configuration_stringstream.str();
}

std::string TuringMachine::GetConfigurationForMarkdown() const {
  std::stringstream configuration_stringstream;
  configuration_stringstream << ';';
  // cannot use for-each loop here since index is necessary
  for (size_t i = 0; i < tape_.size(); i++) {
    if (i == index_of_scanner_) {
      // NOTE: 'q' always precedes the name of the state; we only want the name 
      // of the state in the subscript
      configuration_stringstream << 'q';
      const std::string kStateName = current_state_.GetStateName();
      const std::string kStateNameWithoutQ = kStateName.substr(1, 
          kStateName.size());
      // NOTE: <sub> is the markdown subscript tag
      configuration_stringstream << "<sub>" << kStateNameWithoutQ << "</sub>";
    }
    configuration_stringstream << tape_.at(i);
  }
  return configuration_stringstream.str();
}

void TuringMachine::Update() {
  std::vector<Direction> possible_directions;
  try {
    possible_directions = directions_by_state_map_.at(current_state_);
  } catch (const std::exception &kException) {
    // if there are no directions for the state, then there is nothing to update
    return;
  }
  
  for (const Direction &kDirection : possible_directions) {
    if (kDirection.GetRead() == tape_.at(index_of_scanner_)) {
      ExecuteDirection(kDirection);
      break; // once the direction is found and executed, nothing to search for
    }
  }
}

void TuringMachine::ExecuteDirection(const Direction &direction) {
  // write the character given by the direction to the tape
  tape_.at(index_of_scanner_) = direction.GetWrite();
  
  // move the scanner 1 index left/right and update the tape size if needed
  const char kScannerMovement = direction.GetScannerMovement();
  const char kLeftMovement = 'l';
  if (kScannerMovement == kLeftMovement) {
    // if the scanner is at the beginning of the tape, insert a blank character
    // to index 0 in order to move left, otherwise move the scanner left (index
    // - 1)
    if (index_of_scanner_ == 0) {
      tape_.insert(tape_.begin(), blank_character_);
    } else {
      index_of_scanner_ -= 1;
    }
  }
  const char kRightMovement = 'r';
  if (kScannerMovement == kRightMovement) {
    index_of_scanner_ += 1;
    // if the updated scanner index is larger than the length of the tape,
    // append a blank character to the tape in order to move right
    if (index_of_scanner_ >= tape_.size()) {
      tape_.push_back(blank_character_);
    }
  }
  
  // update the current state and halt the turing machine if the current state 
  // is now a halting state
  current_state_ = direction.GetStateToMoveTo();
  if (std::find(halting_state_names_.begin(),
      halting_state_names_.end(), current_state_.GetStateName()) 
      != halting_state_names_.end()) {
    is_halted_ = true;
  }
}

} // namespace turingmachinesimulator
