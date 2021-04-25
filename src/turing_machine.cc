#include "turing_machine.h"

namespace turingmachinesimulator {

TuringMachine::TuringMachine(const std::vector<State> &kStates,
    const std::vector<Direction> &kDirections, const std::vector<char> &kTape) 
    : tape_(kTape) {
  // set starting and halting states
  std::vector<std::string> state_names;
  for (const State &kState : kStates) {
    if (kState.GetStateName() == "q1") {
      if (!starting_state_.IsEmpty()) {
        error_message_ = "Cannot Have More Than 1 Starting State";
        return;
      } else {
        starting_state_ = kState;
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
  if (starting_state_.IsEmpty() || halting_states_.empty()) {
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

State TuringMachine::GetStartingState() const {
  return starting_state_;
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

} // namespace turingmachinesimulator
