#pragma once

#include "cinder/gl/gl.h"

#include <string>

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
     * @param kStateName a string representing the name of the state
     * @param kStateLocation a vec2 representing the location of the state in 
     *     the simulator app
     * @param radius a double representing the radius of the state in the 
     *     simulator app
     */
    State(int id, const std::string &kStateName, 
        const glm::vec2 &kStateLocation, double radius);
    
    void SetStateName(const std::string &kStateName);
    
    std::string GetStateName() const;
    
    void SetStateLocation(const glm::vec2 &kStateLocation);
    
    glm::vec2 GetStateLocation() const;
    
    int GetId() const;
    
    /**
     * This method takes in a location and radius and returns true if the state
     * is within the radius of the given location
     * 
     * @param kLocation a vec2 representing a point
     * @param radius a double representing a radius to see if the given location 
     *     is within
     * @return a bool that is true if the state is within the given radius of
     *     the given point
     */
    bool StateCenterIsWithinGivenRadius(const glm::vec2 &kLocation,
        double radius) const; 
    
    /**
     * This method displays the state in the app
     */
    void Display() const;
    
    /**
     * This method returns true if the state is empty (no other variables are
     * initialized) and false otherwise
     * 
     * @return true if the state object is empty and false otherwise
     */
    bool IsEmpty() const;
    
    /**
     * This method compares this state object with another state object to 
     * see if the 2 states are equal (have the same id)
     * 
     * @param kState a State object representing a kState to compare with this 
     *     state
     * @return true if the 2 states are equal and false otherwise
     */
    bool Equals(const State &kState) const;
    
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
    
    glm::vec2 state_location_; // a vec2 storing the location of the state
    
    double radius_; // a double storing the radius of the state
    
    /**
     * bool storing whether or not the state object is empty (no other variables 
     * are initialized than is_empty_)
     */
    bool is_empty_ = true;
};

} // namespace turingmachinesimulator
