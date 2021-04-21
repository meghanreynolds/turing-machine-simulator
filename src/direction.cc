#include "direction.h"

namespace turingmachinesimulator {

Direction::Direction(const std::vector<std::string> &kData, 
    const std::vector<State> &kPossibleStates) {
  const size_t kExpectedNumStringInputs = 5;
  if (kData.size() != kExpectedNumStringInputs) {
    // do not create a non-empty direction if given wrong number data strings
    return;
  }
  
  // validate read and write directions
  const size_t kIndexOfReadInput = 0;
  const size_t kIndexOfWriteInput = 1;
  const std::string kRead = kData[kIndexOfReadInput];
  const std::string kWrite = kData[kIndexOfWriteInput];
  if (kRead.size() != 1 || kWrite.size() != 1) {
    // do not create a non-empty direction if given more/less than 1 char for 
    // the read or write condition
    return;
  }

  // validate direction to move scanner
  const size_t kIndexOfScannerMovement = 2;
  const std::string kScannerMovement = kData[kIndexOfScannerMovement];
  if (kScannerMovement.size() != 1) {
    // do not create a non-empty direction if given more/less than 1 char for
    // the scanner movement direction
    return;
  }
  // scanner movement must have size 1 at this point, so it's character is at 
  // index 0
  const char kScannerMovementChar = std::tolower(kScannerMovement.at(0));
  // do not create a non-empty direction if scanner movement character is not 
  // l, r, or n
  if (kScannerMovementChar != 'l' && kScannerMovementChar != 'r' 
      && kScannerMovementChar != 'n') {
    return;
  }

  // validate move from and move to states
  const size_t kIndexOfMoveFromName = 3;
  const std::string kMoveFromName = kData[kIndexOfMoveFromName];
  const size_t kIndexOfMoveToName = 4;
  const std::string kMoveToName = kData[kIndexOfMoveToName];
  State move_from_state = State();
  State move_to_state = State();
  for (const State &kState : kPossibleStates) {
    // NOTE: not in an else-if statement because a direction may keep the same 
    // state, in which case the move from and move to states would be the same
    if (kState.GetStateName() == kMoveFromName) {
      move_from_state = kState;
    }
    if (kState.GetStateName() == kMoveToName) {
      move_to_state = kState;
    }
  }
  if (move_from_state.IsEmpty() || move_to_state.IsEmpty()) {
    // do not create non-empty direction if the move from or move to state does
    // not exist
    return;
  }
  
  // if all data given was good, create a non-empty direction object
  const size_t kIndexOfReadWriteChars = 0;
  read_ = kRead.at(kIndexOfReadWriteChars);
  write_ = kWrite.at(kIndexOfReadWriteChars);
  scanner_movement_ = kScannerMovementChar;
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

