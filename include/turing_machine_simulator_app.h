#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "state.h"

#include <vector>

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

  private:
    const int kWindowSize = 850; // int storing the app window size
    
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
    const int kXLocationOfExampleStates = (int) kWindowSize - 
        (kRadiusOfStates * 2);

    /**
     * vec2 storing the coordinates of the center of the example starting state
     */
    const glm::vec2 kLocationOfQ1 = glm::vec2(kXLocationOfExampleStates, 
        (int) kWindowSize / 6);
    
    /**
     * vec2 storing the coordinates of the center of the example nth state
     */
    const glm::vec2 kLocationOfQn = glm::vec2(kXLocationOfExampleStates, 
        (int) 3 * (kWindowSize / 6));
    
    /**
     * vec2 storing the coordinates of the center of the example halting state
     */
    const glm::vec2 kLocationOfQh = glm::vec2(kXLocationOfExampleStates, 
        (int) 5 * (kWindowSize / 6));

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
     * int storing an id to assign each state created by the user (incremented 
     * after the creation of each new state)
     */
    int state_id_ = 0;
    
    /**
     * vector storing all the user-defined states
     */
    std::vector<State> states_ = {};
    
    /**
     * State storing the state most recently clicked by the user
     */
    State clicked_state_;
};

} // namespace turingmachinesimulator
