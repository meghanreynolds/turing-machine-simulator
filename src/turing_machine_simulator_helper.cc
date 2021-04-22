#include "turing_machine_simulator_helper.h"

namespace turingmachinesimulator {

bool TuringMachineSimulatorHelper::IsPointInRectangle(const glm::vec2 
    &kPoint, const ci::Rectf &kRectangle) {
  const glm::vec2 kRectangleUpperLeftCorner = kRectangle.getUpperLeft();
  const glm::vec2 kRectangleLowerRightCorner = kRectangle.getLowerRight();
  if (kPoint.x >= kRectangleUpperLeftCorner.x && kPoint.x
      <= kRectangleLowerRightCorner.x) {
    if (kPoint.y >= kRectangleUpperLeftCorner.y && kPoint.y <=
        kRectangleLowerRightCorner.y) {
      return true;
    }
  }
  return false;
}

glm::vec2 TuringMachineSimulatorHelper::GetDirectionTextLocation(const glm::vec2 
    &kPointA, const glm::vec2 &kPointB, double state_radius) {
  const int kSpaceFromLine = 10;
  if (kPointB == kPointA) {
    return glm::vec2(kPointA.x, kPointA.y - state_radius - kSpaceFromLine);
  }
  const double kDistanceForOverlappingStates = state_radius * 2;
  if (glm::distance(kPointA, kPointB) <= kDistanceForOverlappingStates) {
    return glm::vec2(kPointA.x, kPointA.y - state_radius - kSpaceFromLine);
  }
  // divide by 2 to get midpoint
  const double kHorizontalMidpoint = (kPointA.x + kPointB.x) / 2; 
  const double kVerticalMidpoint = (kPointA.y + kPointB.y) / 2;
  return glm::vec2(kHorizontalMidpoint, kVerticalMidpoint - kSpaceFromLine);
}

size_t TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked(const 
    glm::vec2 &kClickedPoint, const glm::vec2 &kTapeUpperCorner,
    const glm::vec2 &kTapeLowerCorner) {
  // users are only allowed to edit tape of size 8
  // the size of the tape is returned when no squares were found to have been
  // clicked
  const size_t kTapeSize = 8;
  if (!IsPointInRectangle(kClickedPoint, ci::Rectf(kTapeUpperCorner, 
      kTapeLowerCorner))) {
    return kTapeSize; 
  }
  const double kTapePixelLength = kTapeLowerCorner.x - kTapeUpperCorner.x;
  const double kSquareHorizontalSize = kTapePixelLength / kTapeSize;
  glm::vec2 square_upper_corner = kTapeUpperCorner;
  glm::vec2 square_lower_corner = glm::vec2(kTapeUpperCorner.x 
      + kSquareHorizontalSize, kTapeLowerCorner.y);
  size_t index_of_square = 0;
  while (index_of_square < 9) {
    const ci::Rectf kSquare = ci::Rectf(square_upper_corner, square_lower_corner);
    if (IsPointInRectangle(kClickedPoint, kSquare)) {
      return index_of_square;
    }
    index_of_square += 1;
    square_upper_corner.x = square_lower_corner.x;
    square_lower_corner.x = square_lower_corner.x + kSquareHorizontalSize;
  }
  return kTapeSize;
}

} // namespace turingmachinesimulator
