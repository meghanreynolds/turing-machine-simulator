#include "turing_machine_simulator_helper_methods.h"

namespace turingmachinesimulator {

bool TuringMachineSimulatorHelperMethods::IsPointInRectangle(const glm::vec2 
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

} // namespace turingmachinesimulator