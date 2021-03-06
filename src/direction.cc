#include "direction.h"

namespace turingmachinesimulator {

Direction::Direction(char read, char write, char move, const State 
    &state_to_move_from, const State &state_to_move_to) {
  read_ = read;
  write_ = write;
  
  // validate scanner movement character (must be l/r/n)
  char kScannerMovementChar = std::tolower(move);
  if (kScannerMovementChar == 'l' || kScannerMovementChar == 'r' 
      || kScannerMovementChar == 'n') {
    scanner_movement_ = kScannerMovementChar;
  } else {
    // don't create non-empty direction object if scanner movement direction is
    // wrong
    return;
  }
  
  // validate state to move from/to
  if (state_to_move_from.IsEmpty() || state_to_move_to.IsEmpty()) {
    // don't create non-empty direction object if state to move from/to is 
    // empty
    return;
  }
  state_to_move_to_ = state_to_move_to;
  state_to_move_from_ = state_to_move_from;
  // if all variables are successfully initialized, then the direction is 
  // non-empty
  is_empty_ = false;
}

Direction::Direction(const std::vector<std::string> &data, const 
    std::vector<State> &possible_states) {
  const size_t kExpectedNumStringInputs = 5;
  if (data.size() != kExpectedNumStringInputs) {
    // do not create a non-empty direction if given wrong number data strings
    return;
  }
  
  // validate read and write directions
  const size_t kIndexOfReadInput = 0;
  const size_t kIndexOfWriteInput = 1;
  const std::string kRead = data[kIndexOfReadInput];
  const std::string kWrite = data[kIndexOfWriteInput];
  if (kRead.size() != 1 || kWrite.size() != 1) {
    // do not create a non-empty direction if given more/less than 1 char for 
    // the read condition or character to write
    return;
  }

  // validate direction to move scanner
  const size_t kIndexOfScannerMovement = 2;
  const std::string kScannerMovement = data[kIndexOfScannerMovement];
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

  // validate move from/to states
  const size_t kIndexOfMoveFromName = 3;
  const std::string kMoveFromName = data[kIndexOfMoveFromName];
  const size_t kIndexOfMoveToName = 4;
  const std::string kMoveToName = data[kIndexOfMoveToName];
  State move_from_state = State();
  State move_to_state = State();
  for (const State &kState : possible_states) {
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
  
  // if all data given was correct, create a non-empty direction object
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

void Direction::SetStateToMoveFrom(const State &state_to_move_from) {
  state_to_move_from_ = state_to_move_from;
}

State Direction::GetStateToMoveFrom() const {
  return state_to_move_from_;
}

void Direction::SetStateToMoveTo(const State &state_to_move_to) {
  state_to_move_to_ = state_to_move_to;
}

State Direction::GetStateToMoveTo() const {
  return state_to_move_to_;
}

bool Direction::operator==(const Direction &direction) const {
  if (read_ == direction.GetRead()) {
    return true;
  }
  return false;
}

bool Direction::Equals(const Direction &direction) const {
  if (direction.IsEmpty()) {
    // if both directions are empty, then they're equal, otherwise they're not
    return is_empty_;
  }
  if (read_ == direction.GetRead() && write_ == direction.GetWrite()
      && scanner_movement_ == direction.GetScannerMovement()
      && state_to_move_from_.Equals(direction.GetStateToMoveFrom())
      && state_to_move_to_.Equals(direction.GetStateToMoveTo())) {
    return true;
  }
  return false;
}

std::string Direction::ToString() const {
  std::stringstream direction_as_stringstream;
  direction_as_stringstream << read_ << ", " << write_ << ", "
      << (char) std::toupper(scanner_movement_);
  return direction_as_stringstream.str();
}

} // namespace turingmachinesimulator
