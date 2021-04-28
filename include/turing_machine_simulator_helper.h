#pragma once

#include <cmath>

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
};

} // namespace turingmachinesimulator
