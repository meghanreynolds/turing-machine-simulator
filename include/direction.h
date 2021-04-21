#pragma once

#include <string>

#include "state.h"

namespace turingmachinesimulator {

/**
 * Class representing a Direction for a Turing Machine
 */
class Direction {
  public:
    /**
     * Default constructor
     */
    Direction() = default;
    
    /**
     * This method creates a Direction Object from a vector of string inputs 
     * with: the read input at index 0, the write input at index 1, the shift
     * input at index 2, the name of the state to move from at index 3, and the
     * name for the state to move to at index 4; and a vector of possible states
     * for the move from/to states
     * 
     * @param kData a vector of strings representing the read, write, and 
     *     shift characters, the name of the state to move from, and the name
     *     of the state to move to
     * @param kPossibleStates a vector of States representing the possible states
     *     for the move to/from state of the direction
     */
    Direction(const std::vector<std::string> &kData, const std::vector<State> 
        &kPossibleStates);
    
    /**
     * This method returns true if the Direction Object is empty (doesn't have
     * any variables initialized other than is_empty_)
     * 
     * @return a bool that is true if the Direction is empty
     */
    bool IsEmpty() const;
    
    char GetRead() const;
    
    char GetWrite() const;
    
    char GetScannerMovement() const;
    
    State GetStateToMoveFrom() const;
    
    State GetStateToMoveTo() const;
    
  private:
    /**
     * Character representing the character that must be read for this direction 
     * to apply
     */
    char read_;
    
    /**
     * Character representing the character to write on the tape given that the
     * read condition is met
     */
    char write_;
    
    /**
     * Character representing how the scanner should move: 
     * l for left
     * r for right
     * n for no movement
     */
    char scanner_movement_;
    
    /**
     * State representing the state the machine must be in in order for the 
     * direction to apply
     */
    State state_to_move_from_;
    
    /**
     * State representing the state to move to after the direction is executed
     */
    State state_to_move_to_;
    
    /**
     * bool that is true if the direction object is empty (its other variables
     * are uninitialized)
     */
    bool is_empty_ = true;
};

} // namespace turingmachinesimulator
