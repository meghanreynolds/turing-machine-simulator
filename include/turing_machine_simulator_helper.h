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
};

} // namespace turingmachinesimulator
