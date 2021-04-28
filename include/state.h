#pragma once

#include <string>

#include "cinder/gl/gl.h"

namespace turingmachinesimulator {

/**
 * Class representing a State of a Turing Machine
 */
class State {
  public: 
    /**
     * Default constructor
     */
    State() = default;

    /**
     * This method creates a new State
     * 
     * @param id an int representing a unique id given to each state (no states
     *     in the same Turing Machine should have the same id)
     * @param state_name a string representing the name of the state
     * @param state_location a vec2 representing the location of the state in 
     *     the simulator app
     * @param radius a double representing the radius of the state in the 
     *     simulator app
     */
    State(int id, const std::string &state_name, const glm::vec2 &state_location, 
        double radius);

    int GetId() const;
    
    void SetStateName(const std::string &state_name);
    
    std::string GetStateName() const;
    
    void SetStateLocation(const glm::vec2 &state_location);
    
    glm::vec2 GetStateLocation() const;

    /**
     * This method returns true if the state is empty (initialized via default
     * constructor) and false otherwise
     * 
     * @return a bool that is true if the state object is empty and false 
     *     otherwise
     */
    bool IsEmpty() const;
    
    /**
     * This method overrides the less than operator to compare states by id's
     * NOTE: This method is only overridden to allow for states to serve
     * as map keys
     * 
     * @param state a State to compare with this state
     * @return a bool that is true if this state's id is less than the id of the 
     *     given state
     */
    bool operator<(const State &state) const;

    /**
     * This method compares this state object with another state object to 
     * see if the 2 states are equal (have the same id)
     * 
     * @param state a State object representing a state to compare with this 
     *     state
     * @return a bool that is true if the 2 states are equal(same ids) and 
     *     false otherwise
     */
    bool Equals(const State &state) const;
    
    /**
     * This method takes in a point and radius and returns true if the state's
     * center is within the radius of the given point
     * 
     * @param point a vec2 representing a point
     * @param radius a double representing a radius to see if the given location 
     *     is within
     * @return a bool that is true if the state is within the given radius of
     *     the given point
     */
    bool IsStateCenterWithinGivenRadiusOfPoint(const glm::vec2 &point,
        double radius) const;

    /**
     * This method displays the state in the simulator app
     */
    void Display() const;
    
  private:
    /**
     * This method draws a starting state
     */
    void DrawStartingState() const;

    /**
     * This method draws an nth state
     */
    void DrawNthState() const;

    /**
     * This method draws a halting state
     */
    void DrawHaltingState() const;
    
    int id_; // an int storing the unique id of the state
    
    std::string state_name_; // a string storing the name of the the state
    
    /**
     * a vec2 storing the location of the center of the state in the simulator
     * app
     */
    glm::vec2 state_location_;
    
    /**
     * a double storing the radius of the state in the simulator app
     */
    double radius_;
    
    /**
     * a bool storing whether or not the state object is empty 
     * (initialized with the default constructor)
     */
    bool is_empty_ = true;
};

} // namespace turingmachinesimulator
