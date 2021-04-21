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

} // namespace turingmachinesimulator
