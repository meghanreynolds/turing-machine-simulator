#include "direction.h"

namespace turingmachinesimulator {

Direction::Direction(const std::vector<std::string> &kInputs, 
    const std::vector<State> &kPossibleStates) {
  const size_t kExpectedNumInputs = 5;
  if (kInputs.size() != kExpectedNumInputs) {
    return;
  }
  // validate read and write inputs
  const size_t kIndexOfReadInput = 0;
  const size_t kIndexOfWriteInput = 1;
  const std::string kRead = kInputs[kIndexOfReadInput];
  const std::string kWrite = kInputs[kIndexOfWriteInput];
  if (kRead.size() != 1 || kWrite.size() != 1) {
    return;
  }

  // validate shift character
  const size_t kIndexOfShiftInput = 2;
  const std::string kShift = kInputs[kIndexOfShiftInput];
  if (kShift.size() != 1) {
    return;
  }
  // shift must have size 1 so it's character is at index 0
  const char kShiftChar = std::tolower(kShift.at(0));
  // shift character must be l, r, or n
  if (kShiftChar != 'l' && kShiftChar != 'r'
      && kShiftChar != 'n') {
    return;
  }

  // validate move from and move to states
  const size_t kIndexOfMoveFromName = 3;
  const std::string kMoveFromName = kInputs[kIndexOfMoveFromName];
  const size_t kIndexOfMoveToName = 4;
  const std::string kMoveToName = kInputs[kIndexOfMoveToName];
  State move_from_state = State();
  State move_to_state= State();
  for (const State &state : kPossibleStates) {
    if (state.GetStateName() == kMoveFromName) {
      move_from_state = state;
    }
    if (state.GetStateName() == kMoveToName) {
      move_to_state = state;
    }
  }
  if (move_from_state.IsEmpty() || move_to_state.IsEmpty()) {
    return;
  }
  
  // if all values given are good, create a non-empty direction
  read_ = kRead.at(0);
  write_ = kWrite.at(0);
  scanner_movement_ = kShiftChar;
  state_to_move_from_ = move_from_state;
  state_to_move_to_ = move_to_state;
  is_empty_ = false;
}

bool Direction::IsEmpty() const{
  return  is_empty_;
}

char Direction::GetRead() const {
  return read_;
}

char Direction::GetWrite() const {
  return write_;
}

char Direction::GetScannerMovement() const {
  return scanner_movement_;
}

State Direction::GetStateToMoveFrom() const {
  return state_to_move_from_;
}

State Direction::GetStateToMoveTo() const {
  return state_to_move_to_;
}

} // namespace turingmachinesimulator

