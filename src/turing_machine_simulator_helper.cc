#include "turing_machine_simulator_helper.h"

namespace turingmachinesimulator {

bool TuringMachineSimulatorHelper::IsPointInRectangle(const glm::vec2 
    &point, const ci::Rectf &rectangle) {
  const glm::vec2 kRectangleUpperLeftCorner = rectangle.getUpperLeft();
  const glm::vec2 kRectangleLowerRightCorner = rectangle.getLowerRight();
  if (point.x >= kRectangleUpperLeftCorner.x && point.x
      <= kRectangleLowerRightCorner.x) {
    if (point.y >= kRectangleUpperLeftCorner.y && point.y <=
        kRectangleLowerRightCorner.y) {
      return true;
    }
  }
  return false;
}

glm::vec2 TuringMachineSimulatorHelper::GetCenterOfRectangle(const ci::Rectf 
    &rectangle) {
  // the variables below use division by 2 to get the midpoint
  const double kHorizontalCenterOfRectangle = (rectangle.getUpperLeft().x 
      + rectangle.getLowerRight().x) / 2;
  const double kVerticalCenterOfRectangle = (rectangle.getUpperLeft().y
      + rectangle.getLowerRight().y) / 2;
  return glm::vec2(kHorizontalCenterOfRectangle, kVerticalCenterOfRectangle);
}

glm::vec2 TuringMachineSimulatorHelper::GetDirectionTextLocation(const glm::vec2 
    &point_a, const glm::vec2 &point_b, double state_radius) {
  const int kSpaceFromLine = 10;
  if (point_b == point_a) {
    return glm::vec2(point_a.x, point_a.y - state_radius - kSpaceFromLine);
  }
  const double kDistanceForOverlappingStates = state_radius * 2;
  if (glm::distance(point_a, point_b) <= kDistanceForOverlappingStates) {
    return glm::vec2(point_a.x, point_a.y - state_radius - kSpaceFromLine);
  }
  // divide by 2 to get midpoint
  const double kHorizontalMidpoint = (point_a.x + point_b.x) / 2; 
  const double kVerticalMidpoint = (point_a.y + point_b.y) / 2;
  return glm::vec2(kHorizontalMidpoint, kVerticalMidpoint - kSpaceFromLine);
}

size_t TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked(
    const glm::vec2 &clicked_point, size_t tape_length,
    const glm::vec2 &tape_upper_corner, const glm::vec2 &tape_lower_corner) {
  // the size of the tape is returned when no squares were found to have been
  // clicked
  if (!IsPointInRectangle(clicked_point, ci::Rectf(tape_upper_corner,
      tape_lower_corner))) {
    return tape_length; 
  }
  const double kTapePixelLength = tape_lower_corner.x - tape_upper_corner.x;
  const double kSquareHorizontalSize = kTapePixelLength / tape_length;
  glm::vec2 square_upper_corner = tape_upper_corner;
  glm::vec2 square_lower_corner = glm::vec2(tape_upper_corner.x
      + kSquareHorizontalSize, tape_lower_corner.y);
  size_t index_of_square = 0;
  while (index_of_square < tape_length) {
    const ci::Rectf kSquare = ci::Rectf(square_upper_corner, square_lower_corner);
    if (IsPointInRectangle(clicked_point, kSquare)) {
      return index_of_square;
    }
    index_of_square += 1;
    square_upper_corner.x = square_lower_corner.x;
    square_lower_corner.x = square_lower_corner.x + kSquareHorizontalSize;
  }
  return tape_length;
}

std::tuple<glm::vec2, glm::vec2, glm::vec2> TuringMachineSimulatorHelper
    ::GetArrow(const glm::vec2 &point_a, const glm::vec2 &point_b) {
  const double kHorizontalMidpoint = (point_a.x + point_b.x) / 2;
  const double kVerticalMidpoint = (point_a.y + point_b.y) / 2;
  const glm::vec2 kTipOfArrow = glm::vec2(kHorizontalMidpoint, 
      kVerticalMidpoint);
  
  // Lines 81-97 adapted from:
  // http://kapo-cpp.blogspot.com/2008/10/drawing-arrows-with-cairo.html
  const double kArrowLineLength = 10;
  const double kArrowInternalAngle = 20;
  const double kArrowAngle = std::atan2(point_b.y - point_a.y, 
      point_b.x - point_a.x) + M_PI;
  
  const double kFirstPointXValue = kTipOfArrow.x + kArrowLineLength
      * std::cos(kArrowAngle - kArrowInternalAngle);
  const double kFirstPointYValue = kTipOfArrow.y + kArrowLineLength 
      * std::sin(kArrowAngle - kArrowInternalAngle);
  const glm::vec2 kFirstPoint = glm::vec2(kFirstPointXValue, kFirstPointYValue);
  
  const double kThirdPointXValue = kTipOfArrow.x + kArrowLineLength 
      * std::cos(kArrowAngle + kArrowInternalAngle);
  const double kThirdPointYValue = kTipOfArrow.y + kArrowLineLength 
      * std::sin(kArrowAngle + kArrowInternalAngle);
  const glm::vec2 kThirdPoint = glm::vec2(kThirdPointXValue, 
      kThirdPointYValue);
  
  return std::tuple<glm::vec2, glm::vec2, glm::vec2>(kFirstPoint, kTipOfArrow,
      kThirdPoint);
}

} // namespace turingmachinesimulator
