#include "direction.h"

namespace turingmachinesimulator {

Direction::Direction(char read, char write, char move, 
    const State &kStateToMoveFrom, const State &kStateToMoveTo) {
  read_ = read;
  write_ = write;
  char kMoveChar = std::tolower(move);
  if (kMoveChar == 'l' || kMoveChar == 'r' || kMoveChar == 'n') {
    scanner_movement_ = kMoveChar;
  } else {
    // don't create non-empty direction object if scanner movement direction is
    // wrong (not l, r, or n)
    return;
  }
  if (kStateToMoveFrom.IsEmpty() || kStateToMoveTo.IsEmpty() 
      || kStateToMoveFrom.GetStateName() == "qh") {
    // don't create non-empty direction object state to move from/to is empty or
    // if state to move from is the halting state
    return;
  }
  state_to_move_to_ = kStateToMoveTo;
  state_to_move_from_ = kStateToMoveFrom;
  is_empty_ = false;
  return;
}

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
  // do not create a non-empty direction if move from state is the halting state
  // because it is impossible to change states from the halting state
  if (kMoveFromName == "qh") {
    return;
  }
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

bool Direction::operator==(const Direction &kDirection) const {
  if (read_ == kDirection.GetRead()) {
    return true;
  }
  return false;
}

bool Direction::Equals(const Direction &kDirection) const {
  if (kDirection.IsEmpty()) {
    // if both directions are empty then they're equal, otherwise they're not
    return is_empty_;
  }
  if (read_ == kDirection.GetRead() && write_ == kDirection.GetWrite() 
      && scanner_movement_ == kDirection.GetScannerMovement() 
      && state_to_move_from_.Equals(kDirection.GetStateToMoveFrom()) 
      && state_to_move_to_.Equals(kDirection.GetStateToMoveTo())) {
    return true;
  }
  return false;
}

std::string Direction::ToString() const {
  std::stringstream direction_as_string;
  direction_as_string << read_ << ", " << write_ << ", " 
      << (char) std::toupper(scanner_movement_);
  return direction_as_string.str();
}

} // namespace turingmachinesimulator
