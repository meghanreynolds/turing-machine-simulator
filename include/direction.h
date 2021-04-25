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
     * This method creates a Direction Object from read, write, and scanner 
     * movement chars as well as a state to move from and a state to move to
     * 
     * @param read a char representing the character to be read for the 
     *     direction to apply
     * @param write a char representing the character for the scanner to write
     * @param move a char representing the direction for the scanner to move in;
     *     should be l, r, or n
     * @param kStateToMoveFrom a State representing the state to move from
     * @param kStateToMoveTo a State representing the state to move to
     */
    Direction(char read, char write, char move, const State &kStateToMoveFrom, 
        const State &kStateToMoveTo);
    
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
    
    /**
     * This method overrides the equality operator to compare Direction objects
     * based on the read character condition
     * NOTE: This allows us to use std::find with directions in TuringMachine.cc,
     * as a result this method checks for a more general criteria than the Equals 
     * method; use the Equals method to compare Direction Objects using
     * all variables and not just the read character condition
     * 
     * @param kDirection a Direction object to compare with this Direction
     * @return a bool that is true if both Directions have the same read 
     *     character and false otherwise
     */
    bool operator==(const Direction &kDirection) const;
    
    /**
     * This method checks to see if 2 Direction objects are the same, meaning
     * that all of it's global variables are the same
     * NOTE: This method is different than the equality operator override which
     * only checks for read character condition equality
     * 
     * @param kDirection a Direction object to compare with this Direction
     * @return a bool that is true if both Directions have all the same global 
     *     variables and false otherwise
     */
    bool Equals(const Direction &kDirection) const;
    
    /**
     * This method returns the string representation of the Direction Object
     * 
     * @return a string representing the Direction Object
     */
    std::string ToString() const;
    
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
