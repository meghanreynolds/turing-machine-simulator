#pragma once

#include <cmath>

#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "direction.h"
#include "state.h"

namespace turingmachinesimulator {

/**
 * This class stores helper methods for the turing machine simulator app,
 * this allows the methods to be properly tested while also not being placed
 * into classes where they don't belong
 */
class TuringMachineSimulatorHelper {
  public:
    /**
     * Default constructor
     */
    TuringMachineSimulatorHelper() = default;
    
    /**
     * This method takes in a point and a rectangle and returns true if the 
     * given point is inside (or on the border of) the rectangle and false 
     * otherwise
     * 
     * @param point a vec2 representing a point
     * @param rectangle a Rectf representing a rectangle
     * @return a bool that is true if the given point is inside of the given
     *    rectangle and false otherwise
     */
    static bool IsPointInRectangle(const glm::vec2 &point, 
        const ci::Rectf &rectangle);

    /**
     * This method calculates the center of the given rectangle
     * 
     * @param rectangle a Rectf to calculate the center of
     * @return a vec2 representing the point in the center of the given rectangle
     */
    static glm::vec2 GetCenterOfRectangle(const ci::Rectf &rectangle);
    
    /**
     * This method calculates the location of the direction arrow's text
     * 
     * @param point_a the location of the center of the first state
     * @param point_b the location of the center of the second state
     * @param state_radius the radius of the states
     * @return a vec2 representing the location of the direction arrow's text
     */
    static glm::vec2 GetDirectionTextLocation(const glm::vec2 &point_a,
        const glm::vec2 &point_b, double state_radius);
    
    /**
     * This method checks to see if a point of the tape has been clicked and
     * returns the index of the square that was clicked (if square 1 is clicked
     * it returns 0) 
     * the size of the tape is returned if no square on the tape was clicked
     * 
     * @param clicked_point a vec2 representing the point that the user clicked
     * @param tape_length a size_t representing the length of the tape (not the
     *     number of indices)
     * @param tape_upper_corner a vec2 representing the upper left corner of the
     *     tape
     * @param tape_lower_corner a vec2 representing the lower right corner of the
     *     tape
     * @return a size_t representing the index of the square on the tape that
     *     was clicked, or a the size of the tape if no square was clicked
     */
    static size_t GetIndexOfSquareOfTapeClicked(const glm::vec2 &clicked_point,
        size_t tape_length, const glm::vec2 &tape_upper_corner,
        const glm::vec2 &tape_lower_corner);
    
    /**
     * This method takes in the endpoints of a line and returns a triple storing
     * the points of a triangle to make the directional arrow for the line
     * 
     * @param point_a a vec2 representing the endpoint of a line
     * @param point_b a vec2 representing the endpoint of a line
     * @return a triple containing the points of the triangle for the arrow
     */
    static std::tuple<glm::vec2, glm::vec2, glm::vec2> GetArrow(const 
        glm::vec2 &point_a, const glm::vec2 &point_b);
    
    /**
     * This method adds a direction to the state diagram if the provided inputs
     * were correct and does nothing otherwise
     * 
     * @param add_direction_inputs a vector of strings representing the read, 
     *     write, and shift characters, the name of the state to move from, 
     *     and the name of the state to move to (in that order)
     * @param directions a vector of Directions representing the current 
     *     directions in the state diagrams
     * @param states a vector of States representing the current States in the
     *     state diagram
     */
    static void AddDirection(const std::vector<std::string> 
        &add_direction_inputs, std::vector<Direction> &directions, const
        std::vector<State> &states);
    
    /**
     * This method resets the tape back to a tape with 8 squares containing
     * blank characters
     * 
     * @param tape a vector of characters representing the tape
     * @param blank_character the turing machine's blank character
     */
    static void ResetTape(std::vector<char> &tape, char blank_character);
    
    /**
     * This method deletes the given state from the given list of states as well
     * as all directions involving that state
     * 
     * @param state_to_delete a State representing the state to be deleted
     * @param states a vector of States representing the states in the state 
     *     diagram
     * @param directions a vector of Directions representing the directions in
     *     the state diagram
     */
    static void DeleteGivenState(const State &state_to_delete, std::vector<State> 
        &states, std::vector<Direction> &directions);
    
    /**
     * This method updates the state that the user is clicking on's position to 
     * be the location that the user clicked
     * 
     * @param clicked_state a State representing the State that the user is 
     * @param states a vector of States representing the States in the State
     *     diagram
     * @param click_location a vec2 representing the location to move the state
     *     to
     * @param halting_state_names a vector of strings representing the potential 
     *     names of halting states
     */
    static void UpdateStatePosition(State &clicked_state, std::vector<State> 
        &states, const glm::vec2 &click_location, const std::vector<std::string> 
        &halting_state_names);
    
    /**
     * This method updates the tape character that is being edited according
     * to the key entered by the user
     * 
     * @param tape a vector of characters representing the tape
     * @param blank_character a char representing the turing machine's blank
     *     character
     * @param index_of_character_to_edit an int representing the index of the
     *     character to edit
     * @param typed_char a character representing the character the user typed
     * @param event_code an int representing the event code of the key the user
     *     entered
     * @return an int representing the updated character being edited
     */
    static int UpdateTapeCharacter(std::vector<char> &tape, char blank_character,
        int index_of_character_to_edit, char typed_char, int event_code);
    
    /**
     * This method updates the blank character according to the key entered by
     * the user
     * 
     * @param type_char a character representing the character the user typed
     * @param event_code an int representing the event code of the key the user
     *     typed
     * @return a tuple with the updated character at index 0 and a bool at index
     *     1 that is true if the character is still being edited and false 
     *     otherwise
     */
    static std::tuple<char, bool> UpdateBlankCharacter(char type_char, 
        int event_code);

    /**
     * This method updates the name of the given state according to the key 
     * entered by the user
     * 
     * @param states a vector of States representing the states in the state
     *     diagram
     * @param directions a vector of Directions representing the directions in
     *     in the state diagram
     * @param state_being_modified a State representing the state to edit
     * @param typed_char a character representing the character the user typed
     * @param event_code an int representing the event code of of the key that
     *     the user entered
     * @return a bool that is true if the state name is still being edited
     */
    static bool UpdateStateName(std::vector<State> &states,
        std::vector<Direction> &directions, State &state_being_modified,
        char typed_char, int event_code);
    
    /**
     * This method updates the add arrow input box at the given index according
     * to the key that was pressed by the user
     * 
     * @param add_arrow_inputs a vector of string representing the add arrow 
     *     box's inputs
     * @param index_of_input_to_edit the index of the add arrow input to edit
     * @param typed_char a character representing the character the user typed
     * @param event_code an int representing the event code of the key that the
     *     user entered
     * @return an int representing the the updated index being edited
     */
    static int UpdateAddArrowInputs(std::vector<std::string> &add_arrow_inputs, 
        int index_of_input_to_edit, char typed_char, int event_code);
    
    /**
     * This method updates the given directions' move to/from states to match
     * the given state if the move to/from state has the same id as the given 
     * state
     * 
     * @param state a State representing the state to update directions with
     * @param directions a vector of Directions representing the directions to
     *     be updated
     */
    static void UpdateDirections(const State &state, std::vector<Direction> 
        &directions);
};

} // namespace turingmachinesimulator
