#include "turing_machine.h"

namespace turingmachinesimulator {

TuringMachine::TuringMachine(const std::vector<State> &states,
    const std::vector<Direction> &directions, const std::vector<char> &tape) {
  // if the given tape is empty, set it to 1 blank character (same thing as 
  // an empty tape)
  if (tape.empty()) {
    const char kBlankChar = '-';
    tape_ = {kBlankChar};
  } else {
    tape_ = tape;
  }
  
  // set starting and halting states
  for (const State &kState : states) {
    if (kState.GetStateName() == "q1") {
      if (!current_state_.IsEmpty()) {
        error_message_ = "Cannot Have More Than 1 Starting State";
        return;
      } else {
        // the machine starts in the starting state
        current_state_ = kState;
      }
    } else if (kState.GetStateName() == "qh") {
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
 // not empty
 is_empty_ = false;
}

State TuringMachine::GetCurrentState() const {
  return current_state_;
}

std::vector<State> TuringMachine::GetHaltingStates() const {
  return halting_states_;
}

std::vector<char> TuringMachine::GetTape() const {
  return tape_;
}

std::map<State, std::vector<Direction>> TuringMachine::GetDirectionsByStateMap()
    const {
  return directions_by_state_map_;
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
  const char kBlankCharacter = '-';
  const char kLeftMovement = 'l';
  if (kScannerMovement == kLeftMovement) {
    // if the scanner is at the beginning of the tape, insert a blank character
    // to index 0 in order to move left, otherwise move the scanner left (index
    // -1)
    if (index_of_scanner_ == 0) {
      tape_.insert(tape_.begin(), kBlankCharacter);
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
      tape_.push_back(kBlankCharacter);
    }
  }
  
  // update the current state and halt the turing machine if current state is 
  // now a halting state
  current_state_ = direction.GetStateToMoveTo();
  const std::string kHaltingStateName = "qh";
  if (current_state_.GetStateName() == kHaltingStateName) {
    is_halted_ = true;
  }
}

} // namespace turingmachinesimulator
