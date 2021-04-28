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
     * @param states a vector of States representing the states of the turing
     *      machine
     * @param directions a vector of Directions representing the directions for
     *     the turing machine
     * @param tape a vector of chars representing the tape of the turing machine
     */
    TuringMachine(const std::vector<State> &states,
        const std::vector<Direction> &directions, const std::vector<char> &tape);
    
    State GetCurrentState() const;
    
    std::vector<State> GetHaltingStates() const;
    
    std::vector<char> GetTape() const;
    
    std::map<State, std::vector<Direction>> GetDirectionsByStateMap() const;

    size_t GetIndexOfScanner() const;
    
    std::string GetErrorMessage() const;

    bool IsHalted() const;
    
    /**
     * This method returns true if the turing machine is empty (encountered
     * initialization error or was created with the default constructor)
     * 
     * @return a bool that is true if the turing machine is empty
     */
    bool IsEmpty() const;
    
   /**
    * This method updates the Turing Machine by following the directions for the 
    * current state of the turing machine
    */
    void Update();
    
  private:
    /**
     * This method executes the given direction
     */
     void ExecuteDirection(const Direction &direction);
     
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
     * size_ storing the index of the tape that the scanner is reading
     */
    size_t index_of_scanner_ = 0;

    /**
     * string storing the error message of the turing machine
     */
    std::string error_message_ = "";

    /**
     * bool storing whether or not the machine is halted (true if halted, false
     * otherwise)
     */
    bool is_halted_ = false;
    
    /**
     * bool that is true if the turing machine object is not successfully 
     * initialized or if the turing machine object was initialized with the
     * default constructor, and false otherwise
     */
    bool is_empty_ = true;
};

} // namespace turingmachinesimulator
