#pragma once

#include "cinder/gl/gl.h"

namespace turingmachinesimulator {

/**
 * This class stores helper methods for the turing machine simulator app,
 * this allows the methods to be used only by the app and also to be properly 
 * tested
 */
class TuringMachineSimulatorHelperMethods {
  public:
    /**
     * Default constructor
     */
    TuringMachineSimulatorHelperMethods() = default;
    
    /**
     * This method takes in a point and a rectangle and returns true if the 
     * given point is inside the rectangle and false otherwise
     * 
     * @param kPoint a vec2 representing a point
     * @param kRectangle a Rectf representing a rectangle
     * @return true if the given point is inside of the rectangle and false 
     *     otherwise
     */
    static bool IsPointInRectangle(const glm::vec2 &kPoint, 
        const ci::Rectf &kRectangle);
};

} // namespace turingmachinesimulator