#pragma once

#include <algorithm>
#include <map>

#include "direction.h"
#include "state.h"

namespace turingmachinesimulator {

/**
 * This class represents a turing machine
 */
class TuringMachine {
  public:
    /**
     * Default Constructor
     */
    TuringMachine() = default;
    
    /**
     * This method creates a turing machine containing the given states and tape
     * and following the given directions
     * 
     * @param kStates a vector of States representing the states of the turing
     *      machine
     * @param kDirections a vector of Directions representing the directions of
     *     the turing machine
     * @param kTape a vector of chars representing the tape of the turing machine
     */
    TuringMachine(const std::vector<State> &kStates, 
        const std::vector<Direction> &kDirections, 
        const std::vector<char> &kTape);
    
    State GetStartingState() const;
    
    std::vector<State> GetHaltingStates() const;
    
    std::vector<char> GetTape() const;
    
    std::map<State, std::vector<Direction>> GetDirectionsByStateMap() const;
    
    std::string GetErrorMessage() const;
    
    /**
     * This method returns true if the turing machine is empty (encountered
     * initialization error or was created with default constructor)
     * 
     * @return a bool that is true if the turing machine is empty
     */
    bool IsEmpty() const;
    
  private:
    /**
     * State storing the starting state of the turing machine
     */
    State starting_state_ = State();
    
    /**
     * vector of states storing the halting states of the turing machine
     */
    std::vector<State> halting_states_;
    
    /**
     * vector of chars storing the tape of the turing machine
     */
    std::vector<char> tape_;
    
    /**
     * map storing states of the turing machine as the keys and vectors of 
     * directions that move from the key state as the values
     */
    std::map<State, std::vector<Direction>> directions_by_state_map_;

    /**
     * bool that is true if the turing machine object is not successfully 
     * initialized or if the turing machine object was initialized with the
     * default constructor
     */
    bool is_empty_ = true;
    
    /**
     * string storing the error message of the turing machine
     */
    std::string error_message_ = "";
};

} // namespace turingmachinesimulator
