#include "turing_machine.h"

namespace turingmachinesimulator {

TuringMachine::TuringMachine(const std::vector<State> &kStates,
    const std::vector<Direction> &kDirections, const std::vector<char> &kTape) {
  if (kTape.empty()) {
    // if the tape is empty, set it to 1 blank character (same as empty tape)
    tape_ = {'-'};
  } else {
    tape_ = kTape;
  }
  // set starting and halting states
  std::vector<std::string> state_names;
  for (const State &kState : kStates) {
    if (kState.GetStateName() == "q1") {
      if (!current_state_.IsEmpty()) {
        error_message_ = "Cannot Have More Than 1 Starting State";
        return;
      } else {
        current_state_ = kState;
      }
    } else if (kState.GetStateName() == "qh") {
      halting_states_.push_back(kState);
    } else {
      const std::string kStateName = kState.GetStateName();
      if (std::find(state_names.begin(), state_names.end(), kStateName) 
          != state_names.end()) {
        error_message_ = "Cannot Have States With The Same Name";
        return;
      } else {
        state_names.push_back(kStateName);
      }
    }
  }
  if (current_state_.IsEmpty() || halting_states_.empty()) {
    error_message_ = "Must Have Halting and Starting States";
    return;
  }
  
  // put directions into the directions by state map
 for (const Direction &kDirection : kDirections) {
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
     // Don't need to do anything, just continue the code
    }
    directions_by_state_map_[kStateToMoveFrom].push_back(kDirection);
  }
 // if no errors were encountered, the turing machine is not empty
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

std::string TuringMachine::GetErrorMessage() const {
  return error_message_;
}

bool TuringMachine::IsEmpty() const {
  return is_empty_;
}

size_t TuringMachine::GetIndexOfScanner() const {
  return  index_of_scanner_;
}

bool TuringMachine::IsHalted() const {
  return is_halted_;
}

void TuringMachine::Update() {
  std::vector<Direction> possible_directions;
  try {
    possible_directions = directions_by_state_map_.at(current_state_);
  } catch (const std::exception &kException) {
    return; // if there are no directions for the state, nothing to update
  }
  for (const Direction &kDirection : possible_directions) {
    if (kDirection.GetRead() == tape_.at(index_of_scanner_)) {
      ExecuteDirection(kDirection);
      break;
    }
  }
}

void TuringMachine::ExecuteDirection(const Direction &kDirection) {
  // write the character to the tape
  tape_.at(index_of_scanner_) = kDirection.GetWrite();
  // move the scanner and update the tape size if it needs to be updated
  const char kScannerMovement = kDirection.GetScannerMovement();
  if (kScannerMovement == 'l') {
    if (index_of_scanner_ == 0) {
      tape_.insert(tape_.begin(), '-');
    } else {
      index_of_scanner_ -= 1;
    }
  }
  if (kScannerMovement == 'r') {
    index_of_scanner_ += 1;
    if (index_of_scanner_ >= tape_.size()) {
      tape_.push_back('-');
    }
  }
  // update the current state and halt the turing machine if state to move to
  // is a halting state
  current_state_ = kDirection.GetStateToMoveTo();
  if (current_state_.GetStateName() == "qh") {
    is_halted_ = true;
  }
}

} // namespace turingmachinesimulator
