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
    
    State GetCurrentState() const;
    
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
    
    size_t GetIndexOfScanner() const;
    
    bool IsHalted() const;
    
    /**
     * This method updates the Turing Machine by following the directions for
     * the current state of the turing machine
     */
    void Update();
    
  private:
    /**
     * This method executes the given direction
     */
     void ExecuteDirection(const Direction &kDirection);
     
    /**
     * State storing the current state of the turing machine
     */
    State current_state_ = State();
    
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
    
    /**
     * size_ storing the index of the tape that the scanner is reading
     */
    size_t index_of_scanner_ = 0;
    
    /**
     * bool storing whether or not the machine is halted (true if halted, false
     * otherwise)
     */
    bool is_halted_ = false;
};

} // namespace turingmachinesimulator
