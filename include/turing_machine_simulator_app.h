#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "direction.h"
#include "state.h"
#include "turing_machine.h"
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
     * This method overrides the update function of a cinder app to update the
     * turing machine simulation while the simulation is in progress
     */
    void update() override;

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
    * This method takes in the location of the user's click and handles the 
    * event if a box was clicked
    * 
    * @param click_location a vec2 representing where the user clicked
    * @return a bool that is true if a box was clicked, false otherwise
    */
    bool HandleClickedBox(const glm::vec2 &click_location);

    /**
     * This method takes in the location of the user's click and handles the 
     * event if a state was created
     * 
     * @param click_location a vec2 representing where the user clicked
     * @return a bool that is true if a new state was created
     */
    bool HandleStateCreation(const glm::vec2 &click_location);

    /**
     * This method takes in the location of the user's click and handles the 
     * event if the user is trying to delete a state
     * 
     * @param click_location a vec2 representing the location of the user's click
     * @return a bool that is true if a state was deleted, false otherwise
     */
    bool HandleStateDeletion(const glm::vec2 &click_location);

    /**
     * This method takes in a key event and updates the character on the tape
     * that is being edited accordingly
     * 
     * @param key_event a KeyEvent representing the key the user pressed
     */
    void EditTapeCharacter(const ci::app::KeyEvent &key_event);
    
    /**
     * This method stops the simulation
     */
     void StopSimulation();
     
     /**
      * This method displays the settings page
      */
    void DisplaySettingsPage() const;
     
    /**
     * This method draws the add arrow menu to the screen
     */
    void DrawAddArrowMenu() const;

    /**
     * This method draws an arrow on the screen for each user-defined direction
     * 
     * NOTE: this method is inside of this class and not the direction class to 
     * allow users to update state locations even after defining directions 
     * for that state
     * 
     * @param direction a Direction to draw an arrow for
     */
    void DrawArrow(const Direction &direction) const;

    /**
     * This method draws the tape on the screen
     */
    void DrawTape() const;

    /**
     * This method displays the turing machine error message
     */
    void DisplayErrorMessage() const;
    
    /**
     * This method displays the given button with the given label and the 
     * given color
     * 
     * @param button a Rectf representing the button to be drawn
     * @param label a string representing the label for the button
     * @param color a Color reprsenting the color of the button
     */
    void DrawButton(const ci::Rectf &button, const std::string &label, 
        const ci::Color &color) const;
    
    /**
     * int storing the horizontal window size, must be at least 600 
     */
    const int kHorizontalWindowSize = 600;
    
    /**
     * int storing the vertical window size, must be at least 600 
     */
    const int kVerticalWindowSize = 600;
    
    /**
     * string storing the path the file storing the complete configurations 
     * produced by the simulation
     */
     const std::string kPathToCompleteConfigurationFile = 
        "complete-configuration/complete_configuration.md";

    /**
     * vector storing all of the user-defined states
     */
    std::vector<State> states_ = {};

    /**
     * vector storing all of the user-defined directions
     */
    std::vector<Direction> directions_ = {};
    
    /**
     * character storing the turing machine tape's blank character
     */
    char blank_character_ = '0';
    
    /**
     * vector storing the characters on the tape (by default starts with 8
     * blank characters)
     */
    std::vector<char> tape_ = {blank_character_, blank_character_, 
                               blank_character_, blank_character_, 
                               blank_character_, blank_character_,
                               blank_character_, blank_character_};
    
    /**
     * vector storing the strings that can be the names of halting states
     */
     std::vector<std::string> halting_state_names_ = {"qh", "qAccept", "qReject"};
     
   /**
    * TuringMachine storing the user defined Turing Machine
    */
    TuringMachine turing_machine_ = TuringMachine();

    /**
     * int storing an id to assign each state created by the user (incremented 
     * after the creation of each new state)
     */
    int state_id_ = 0;
    
    /**
     * int storing the number of simulations that have been run (regardless of
     * clears)
     */
    int num_simulations_run_ = 0;
     
     /**
     * a boolean that is true if it is the simulation has just begun
     */
    bool is_first_turn_of_simulation_ = true;

    /**
     * a State that stores the halting state that the simulation ended on
     */
    State halting_state_to_highlight_ = State();
    
    /**
     * a size_t tracking the number of times the halting state has been 
     * highlighted
     */
     size_t num_times_halting_state_highlighted_ = 0;
     
    /**
     * bool that is true if a turing machine is being simulated
     */
     bool simulation_is_in_progress_ = false;
     
     /**
      * bool that is true if the settings page is being displayed
      */
     bool settings_is_showing_ = false;
     
     /**
      * bool that is true if the blank character is being edited
      */
     bool editing_blank_char_ = false;

    /**
     * size_t storing the index of the character the scanner is reading
     */
    size_t index_of_character_being_read_ = 0;

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
     * State storing the state most recently clicked by the user, empty state if 
     * the user has not recently clicked a state
     */
    State clicked_state_ = State();

    /**
     * bool storing whether or not a tape character is being edited
     */
    bool editing_tape_character_ = false;
    
    /**
     * size_t storing the index of the character that is being edited
     * This is equal to the size of the tap when there are no characters being 
     * edited
     */
     size_t index_of_character_being_edited_ = tape_.size();
    
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
     * size_t storing the index of the add arrow box input text to edit,
     * value is size of add arrow inputs when no add arrow text is being edited
     */
    size_t index_of_add_arrow_text_to_edit = add_arrow_inputs_.size();
    
    
    // NOTE: All magic numbers found below were found by experimenting
    // for the best visual experience
    
    /**
     * int storing the x boundary for the menu
     */
    const int kMenuXBoundary = (int) kHorizontalWindowSize 
        - (kHorizontalWindowSize / 5);

    /**
     * int storing the radius of the states in the app
     */
    const int kRadiusOfStates = (int) (kHorizontalWindowSize - kMenuXBoundary) 
        / 4;

    /**
     * int soring the x coordinate of the example states
     */
    const int kXLocationOfExampleStates = (int) kHorizontalWindowSize - 
        (kRadiusOfStates * 2);

    /**
     * vec2 storing the coordinates of the center of the example starting state
     */
    const glm::vec2 kLocationOfQ1 = glm::vec2(kXLocationOfExampleStates,
        (int) kVerticalWindowSize / 8);

    /**
     * vec2 storing the coordinates of the center of the example nth state
     */
    const glm::vec2 kLocationOfQn = glm::vec2(kXLocationOfExampleStates,
        (int) 3 * (kVerticalWindowSize / 8));

    /**
     * vec2 storing the coordinates of the center of the example halting state
     */
    const glm::vec2 kLocationOfQh = glm::vec2(kXLocationOfExampleStates,
        (int) 5 * (kVerticalWindowSize / 8));

    /**
     * State storing the example starting state
     */
    const State kExampleStartingState = State(-1, "q1", kLocationOfQ1,
        kRadiusOfStates, halting_state_names_);

    /**
     * State storing the example nth state 
     */
    const State kExampleNthState = State(-1, "qn", kLocationOfQn,
        kRadiusOfStates, halting_state_names_);

    /**
     * State storing the example halting state
     */
    const State kExampleHaltingState = State(-1, "qh", kLocationOfQh,
        kRadiusOfStates, halting_state_names_);
    
    /**
     * int storing the x boundary for the add arrow box
     */
    const int kAddArrowBoxXBoundary = (int) kHorizontalWindowSize - (2 
        * (kHorizontalWindowSize / 5));
    
    /**
     * int storing the y boundary for the add arrow box
     */
    const int kAddArrowBoxYBoundary = (int) 6 * (kVerticalWindowSize / 8);

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
    const glm::vec2 kLowerCornerOfReadInput = glm::vec2(kHorizontalWindowSize,
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
    const glm::vec2 kLowerCornerOfWriteInput = glm::vec2(kHorizontalWindowSize,
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
    const glm::vec2 kLowerCornerOfShiftInput = glm::vec2(kHorizontalWindowSize,
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
    const glm::vec2 kLowerCornerOfMoveFromInput = glm::vec2(kHorizontalWindowSize, 
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
    const glm::vec2 kLowerCornerOfMoveToInput = glm::vec2(kHorizontalWindowSize,
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
    const glm::vec2 kLowerCornerOfSubmit = glm::vec2(kHorizontalWindowSize, 
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
    
    /**
     * vec2 storing the upper left corner of the simulate button
     */
    const glm::vec2 kUpperCornerSimulateButton = glm::vec2(kMenuXBoundary 
        - 90, 10);
     
     /**
      * vec2 storing the lower right corner of the simulate button
      */
    const glm::vec2 kLowerCornerSimulateButton = glm::vec2(kMenuXBoundary
        - 10, 90);
    
    /**
     * vec2 storing the simulate button
     */
     const ci::Rectf kSimulateButton = ci::Rectf(kUpperCornerSimulateButton, 
         kLowerCornerSimulateButton);
    
    /**
     * vec2 storing the upper left corner of the tape
     */
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 
        kUpperCornerOfWriteInput.y);
    
    /**
     * vec2 storing the lower right corner of the tape
     */
    const glm::vec2 kLowerCornerOfTape = glm::vec2(kAddArrowBoxXBoundary - 10, 
        kLowerCornerOfMoveFromInput.y);
    
    /**
     * vec2 storing the upper left corner of the reset tape button 
     */
    const glm::vec2 kUpperCornerOfResetTapeButton = 
        glm::vec2(kUpperCornerOfTape.x , kUpperCornerOfTape.y - 90);
    
    /**
     * vec2 storing the lower right corner of the reset tape button
     */
    const glm::vec2 kLowerCornerOfResetTapeButton = 
        glm::vec2(kUpperCornerOfTape.x + 80, kUpperCornerOfTape.y - 10);
    
    /**
     * Rectf storing the reset tape button
     */
    const ci::Rectf kResetTapeButton = ci::Rectf(kUpperCornerOfResetTapeButton,
        kLowerCornerOfResetTapeButton);
    
    /**
     * vec2 storing the upper left corner of the stop simulation button
     */
    const glm::vec2 kUpperCornerStopButton = glm::vec2(kMenuXBoundary - 90, 
        kAddArrowBoxYBoundary - 90);
    
    /**
     * vec2 storing the lower right corner of the stop simulation button
     */
    const glm::vec2 kLowerCornerStopButton = glm::vec2(kMenuXBoundary - 10, 
        kAddArrowBoxYBoundary - 10);
    
    /**
     * Rectf storing the stop simulation button
     */
    const ci::Rectf kStopSimulationButton = ci::Rectf(kUpperCornerStopButton, 
        kLowerCornerStopButton);
    
    /**
     * vec2 storing the upper left corner of the settings button
     */
    const glm::vec2 kUpperCornerSettingsButton = glm::vec2(kHorizontalWindowSize 
        - 55, 0);
    
    /**
     * vec2 storing the lower right corner of the settings button
     */
    const glm::vec2 kLowerCornerSettingsButton = glm::vec2(kHorizontalWindowSize, 
        40);
    
    /**
     * Rectf storing the settings button
     */
    const ci::Rectf kSettingsButton = ci::Rectf(kUpperCornerSettingsButton, 
        kLowerCornerSettingsButton);
    
    /**
     * vec2 storing the upper left corner of the blank character input box
     */
    const glm::vec2 kUpperCornerBlankCharBox = glm::vec2(110, 30);
    
    /**
     * vec2 storing the lower right corner of the blank character input box
     */
    const glm::vec2 kLowerCornerBlankCharBox = glm::vec2(150, 60);
    
    /**
     * Rect storing the blank character input box
     */
    const ci::Rectf kBlankCharInputBox = ci::Rectf(kUpperCornerBlankCharBox, 
        kLowerCornerBlankCharBox);
    
    /**
     * vec2 storing the upper left corner of the exit settings button
     */
    const glm::vec2 kUpperCornerExitSettingsButton = 
        glm::vec2(kHorizontalWindowSize - 40, 0);
    
    /**
     * vec2 storing the lower right corner of the exit settings button
     */
    const glm::vec2 kLowerCornerExitSettingsButton = 
        glm::vec2(kHorizontalWindowSize, 40);
    
    /**
     * Rectf storing the exit settings button
     */
    const ci::Rectf kExitSettingsButton = 
        ci::Rectf(kUpperCornerExitSettingsButton, kLowerCornerExitSettingsButton);
};

} // namespace turingmachinesimulator
