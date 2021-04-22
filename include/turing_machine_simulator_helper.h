#pragma once

#include "cinder/gl/gl.h"

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
     * @param kPoint a vec2 representing a point
     * @param kRectangle a Rectf representing a rectangle
     * @return a bool that is true if the given point is inside of the given
     *    rectangle and false otherwise
     */
    static bool IsPointInRectangle(const glm::vec2 &kPoint, 
        const ci::Rectf &kRectangle);
    
    /**
     * This method calculates the location of the direction arrow's text
     * 
     * @param kPointA the location of the center of the first state
     * @param kPointB the location of the center of the second state
     * @param state_radius the radius of the states
     * @return a vec2 representing the location of the direction arrow's text
     */
    static glm::vec2 GetDirectionTextLocation(const glm::vec2 &kPointA, 
        const glm::vec2 &kPointB, double state_radius);
    
    /**
     * This method checks to see if a point of the tape has been clicked and
     * returns the index of the square that was clicked (if square 1 is clicked
     * it returns 0) 
     * 8 is returned if no square on the tape was clicked
     * 
     * @param kClickedPoint a vec2 representing the point that the user clicked
     * @param kTapeUpperCorner a vec2 representing the upper left corner of the
     *     tape
     * @param kTapeLowerCorner a vec2 representing the lower right corner of the
     *     tape
     * @return a size_t representing the index of the square on the tape that
     *     was clicked, or 8 if no square was clicked
     */
     static size_t GetIndexOfSquareOfTapeClicked(const glm::vec2 &kClickedPoint, 
         const glm::vec2 &kTapeUpperCorner, const glm::vec2 &kTapeLowerCorner);
};

} // namespace turingmachinesimulator
