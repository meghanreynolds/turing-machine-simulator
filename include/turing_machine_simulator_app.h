#pragma once

#include<algorithm>
#include<string>
#include <vector>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "direction.h"
#include "state.h"
#include "turing_machine_simulator_helper.h"

namespace turingmachinesimulator {

/**
 * App that simulates a Turing Machine created by users
 */
class TuringMachineSimulatorApp : public ci::app::App {
  public:
    /**
     * This method creates a new instance of the Turing Machine Simulator App
     */
    TuringMachineSimulatorApp();
    
    /**
     * This method overrides the draw function of a cinder app to render the
     * Turing Machine Simulator App
     */
    void draw() override;

    /**
     * This method overrides the mouseDown function of a cinder app to respond
     * to user clicks
     * 
     * @param event a MouseEvent containing information about the users' click
     */
    void mouseDown(ci::app::MouseEvent event) override;
    
    /**
     * This method overrides the mouseDrag function of a cinder app to respond
     * to the user dragging the mouse across the screen
     * 
     * @param event a MouseEvent containing information about where the user
     *     dragged their mouse
     */
    void mouseDrag(ci::app::MouseEvent event) override;
    
    /**
     * This method overrides the keyDown function of a cinder app to respond to 
     * the users' keyboard input
     * 
     * @param event a KeyEvent containing information about what the key the user
     *     touched
     */
    void keyDown(ci::app::KeyEvent event) override;

  private:
    /**
     * This method draws the add arrow menu to the screen
     * 
     * NOTE: All magic numbers found in this method were found by experimenting
     * for the most visually appealing appearance of the menu
     */
    void DrawAddArrowMenu() const;
    
    /**
     * This method draws the clear button to the screen
     */
    void DrawClearButton() const;
    
    /**
     * This method draws an arrow on the screen for each user-defined direction
     * 
     * NOTE: this method is inside of this class to all users to update
     * state locations even after defining a directions for that state
     * 
     * @param kDirection a Direction to draw an arrow for
     */
    void DrawArrow(const Direction &kDirection) const;

    /**
     * This method takes in the location of the user's click and handles the 
     * event if a box was clicked
     * 
     * @param kClickLocation a vec2 representing where the user clicked
     * @return a bool that is true if a box was clicked, false otherwise
     */
    bool HandleClickedBox(const glm::vec2 &kClickLocation);

    /**
     * This method takes in the location of the user's click and handles the 
     * event if a state was created
     * 
     * @param kClickLocation a vec2 representing where the user clicked
     * @return a bool that is true if a new state was created
     */
    bool HandleStateCreation(const glm::vec2 &kClickLocation);
    
    /**
     * This method takes in the location of the user's click and handles the 
     * event if the user is trying to delete a state
     * 
     * @param kClickLocation a vec2 representing the location of the user's click
     * @return a bool that is true if a state was deleted, false otherwise
     */
    bool HandleStateDeletion(const glm::vec2 &kClickLocation);
    
    /**
     * This method takes in a key event and updates the state whose name is
     * being edited's name accordingly
     * 
     * @param kKeyEvent a KeyEvent representing the key the user pressed
     */
    void EditStateName(const ci::app::KeyEvent &kKeyEvent);

    /**
     * This method takes in a key event and updates the add arrow input box that
     * is being edited's text accordingly
     * 
     * @param kKeyEvent a KeyEvent representing the key the user pressed
     */
    void EditAddArrowInputBox(const ci::app::KeyEvent &kKeyEvent);
    
    /**
     * int storing the window size, must be at least 600 
     */
    const int kWindowSize = 800;

    /**
     * int storing an id to assign each state created by the user (incremented 
     * after the creation of each new state)
     */
    int state_id_ = 0;

    /**
     * vector storing all of the user-defined states
     */
    std::vector<State> states_ = {};

    /**
     * vector storing all of the user-defined directions
     */
    std::vector<Direction> directions_ = {};

    /**
     * State storing the state most recently clicked by the user, empty state if 
     * the user has not recently clicked a state
     */
    State clicked_state_ = State();
    
    /**
     * bool storing whether or not a state name is currently being edited
     */
    bool editing_state_name_ = false;
    
    /**
     * State storing the state whose name is being modified, empty if a state 
     * name is not currently being edited
     */
    State state_being_modified_ = State();
    
    /**
     * Vector of strings storing:
     * the read input at index 0, 
     * the write input at index 1, 
     * the shift input at index 2, 
     * the state to move from's name at index 3,
     * the state to move to's name at index 4
     */
    std::vector<std::string> add_arrow_inputs_ = {"single char", "single char",
        "L/R/N", "q5", "qh"};
    
    /**
     * size_t storing the index of the add arrow box input text to edit,
     * value is 5 when no add arrow text is being edited
     */
    size_t index_of_add_arrow_text_to_edit = 5;

    /**
     * Turing machine simulator helper object storing methods for use in the 
     * UI Code
     */
    const TuringMachineSimulatorHelper kHelperMethods = 
        TuringMachineSimulatorHelper();
    
    // NOTE: All magic numbers found below were found by experimenting
    // for the most visually appealing appearance of the menu
    
    /**
     * int storing the x boundary for the menu
     */
    const int kMenuXBoundary = (int) kWindowSize - (kWindowSize / 5);

    /**
     * int storing the radius of the states in the app
     */
    const int kRadiusOfStates = (int) (kWindowSize - kMenuXBoundary) / 4;

    /**
     * int soring the x coordinate of the example states
     */
    const int kXLocationOfExampleStates = (int) kWindowSize - (kRadiusOfStates 
        * 2);

    /**
     * vec2 storing the coordinates of the center of the example starting state
     */
    const glm::vec2 kLocationOfQ1 = glm::vec2(kXLocationOfExampleStates,
        (int) kWindowSize / 8);

    /**
     * vec2 storing the coordinates of the center of the example nth state
     */
    const glm::vec2 kLocationOfQn = glm::vec2(kXLocationOfExampleStates,
        (int) 3 * (kWindowSize / 8));

    /**
     * vec2 storing the coordinates of the center of the example halting state
     */
    const glm::vec2 kLocationOfQh = glm::vec2(kXLocationOfExampleStates,
        (int) 5 * (kWindowSize / 8));

    /**
     * State storing the example starting state
     */
    const State kExampleStartingState = State(-1, "q1", kLocationOfQ1,
        kRadiusOfStates);

    /**
     * State storing the example nth state 
     */
    const State kExampleNthState = State(-1, "qn", kLocationOfQn,
        kRadiusOfStates);

    /**
     * State storing the example halting state
     */
    const State kExampleHaltingState = State(-1, "qh", kLocationOfQh,
        kRadiusOfStates);
    
    /**
     * int storing the x boundary for the add arrow box
     */
    const int kAddArrowBoxXBoundary = (int) kWindowSize - (2 * (kWindowSize / 5));
    
    /**
     * int storing the y boundary for the add arrow box
     */
    const int kAddArrowBoxYBoundary = (int) 6 * kWindowSize / 8;

    /**
     * size_t storing the index of read input in add_arrow_inputs_
     */
    const size_t kIndexOfReadInput = 0;
    
    /**
     * size_t storing the index of the write input in add_arrow_inputs_
     */
    const size_t kIndexOfWriteInput = 1;
    
    /**
     * size_t storing the index of the shift input in add_arrow_inputs_
     */
    const size_t kIndexOfShiftInput = 2;
    
    /**
     * size_t storing the index of the move from input in add_arrow_inputs_
     */
    const size_t kIndexOfMoveFromInput = 3;
    
    /**
     * size_t storing the index of the move to input in add_arrow_inputs_
     */
    const size_t kIndexOfMoveToInput = 4;

    /**
     * vec2 storing the upper left corner of the read input box
     */
    const glm::vec2 kUpperCornerOfReadInput = glm::vec2(kAddArrowBoxXBoundary
        + 40, kAddArrowBoxYBoundary + 15); 
    
    /**
     * vec2 storing the lower right corner of the read input box
     */
    const glm::vec2 kLowerCornerOfReadInput = glm::vec2(kWindowSize,
        kAddArrowBoxYBoundary + 35);
    
    /**
     * Rectf storing the read input box
     */
    const ci::Rectf kReadInputBox = ci::Rectf(kUpperCornerOfReadInput, 
        kLowerCornerOfReadInput);
    
    /**
     * vec2 storing the upper left corner of the write input box
     */
    const glm::vec2 kUpperCornerOfWriteInput = glm::vec2(kAddArrowBoxXBoundary
        + 40, kAddArrowBoxYBoundary + 40);
    
    /**
     * vec2 storing the lower right corner of the write input box
     */
    const glm::vec2 kLowerCornerOfWriteInput = glm::vec2(kWindowSize,
        kAddArrowBoxYBoundary + 55);
    
    /**
     * Rectf storing the write input box
     */
    const ci::Rectf kWriteInputBox = ci::Rectf(kUpperCornerOfWriteInput,
        kLowerCornerOfWriteInput);

    /**
     * vec2 storing the upper left corner of the shift input box
     */
    const glm::vec2 kUpperCornerOfShiftInput = glm::vec2(kAddArrowBoxXBoundary
        + 40, kAddArrowBoxYBoundary + 65);
    
    /**
     * vec2 storing the lower right corner of the shift input box
     */
    const glm::vec2 kLowerCornerOfShiftInput = glm::vec2(kWindowSize,
        kAddArrowBoxYBoundary + 80);
    
    /**
     * Rectf storing the shift input box
     */
    const ci::Rectf kShiftInputBox = ci::Rectf(kUpperCornerOfShiftInput,
        kLowerCornerOfShiftInput);

    /**
     * vec2 storing the upper left corner of the move from input box
     */
    const glm::vec2 kUpperCornerOfMoveFromInput = glm::vec2(kAddArrowBoxXBoundary
        + 70, kAddArrowBoxYBoundary + 90);
    
    /**
     * vec2 storing the lower right corner of the move from input box
     */
    const glm::vec2 kLowerCornerOfMoveFromInput = glm::vec2(kWindowSize, 
        kAddArrowBoxYBoundary + 105);
    
    /**
     * Rectf storing the move from input box
     */
    const ci::Rectf kMoveFromInputBox = ci::Rectf(kUpperCornerOfMoveFromInput,
        kLowerCornerOfMoveFromInput);

    /**
     * vec2 storing the upper left corner of the move to input box
     */
    const glm::vec2 kUpperCornerOfMoveToInput = glm::vec2(kAddArrowBoxXBoundary
        + 55, kAddArrowBoxYBoundary + 115);
    
    /**
     * vec2 storing the lower right corner of the move to input box
     */
    const glm::vec2 kLowerCornerOfMoveToInput = glm::vec2(kWindowSize,
        kAddArrowBoxYBoundary + 130);
    
    /**
     * Rectf storing the move to input box
     */
    const ci::Rectf kMoveToInputBox = ci::Rectf(kUpperCornerOfMoveToInput,
        kLowerCornerOfMoveToInput);

    /**
     * vec2 storing the upper left corner of the submit button
     */
    const glm::vec2 kUpperCornerOfSubmit = glm::vec2(kAddArrowBoxXBoundary,
        kAddArrowBoxYBoundary + 135);
    
    /**
     * vec2 storing the lower right corner of the submit button
     */
    const glm::vec2 kLowerCornerOfSubmit = glm::vec2(kWindowSize, 
        kAddArrowBoxYBoundary + 150);
    
    /**
     * Rectf storing the submit button
     */
    const ci::Rectf kSubmitButton = ci::Rectf(kUpperCornerOfSubmit,
        kLowerCornerOfSubmit);

    /**
     * vec2 storing the upper left corner of the clear button
     */
    const glm::vec2 kUpperCornerClearButton = glm::vec2(10, 10);
    
    /**
     * vec2 storing the lower right corner of the clear button
     */
    const glm::vec2 kLowerCornerClearButton = glm::vec2(90, 90);
    
    /**
     * Rectf storing the clear button
     */
    const ci::Rectf kClearButton = ci::Rectf(kUpperCornerClearButton,
        kLowerCornerClearButton);
};

} // namespace turingmachinesimulator
