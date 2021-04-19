#include <catch2/catch.hpp>

#include "turing_machine_simulator_helper_methods.h"

using namespace turingmachinesimulator;

/**
 * Partitions Testing As Follows:
 * Correctly Checks If Given Point Is In Given Rectangle
 */
TEST_CASE("Test Checks If Given Point Is In Given Rectangle") {
  const TuringMachineSimulatorHelperMethods kHelperMethods =
      TuringMachineSimulatorHelperMethods();
  
  SECTION("Point Is Not In Rectangle", "[point is in rectangle]") {
    const glm::vec2 kPoint = glm::vec2(15, 15);
    const ci::Rectf kRectangle = ci::Rectf(glm::vec2(1, 1), 
        glm::vec2(14, 14));
    REQUIRE(kHelperMethods.IsPointInRectangle(kPoint, kRectangle) == false);
  }

  SECTION("Point Is On Edge Of Rectangle", "[point is in rectangle]") {
    const glm::vec2 kPoint = glm::vec2(14, 14);
    const ci::Rectf kRectangle = ci::Rectf(glm::vec2(1, 1),
        glm::vec2(14, 14));
    REQUIRE(kHelperMethods.IsPointInRectangle(kPoint, kRectangle) == true);
  }

  SECTION("Point Is Inside Rectangle", "[point is in rectangle]") {
    const glm::vec2 kPoint = glm::vec2(10, 10);
    const ci::Rectf kRectangle = ci::Rectf(glm::vec2(1, 1),
        glm::vec2(14, 14));
    REQUIRE(kHelperMethods.IsPointInRectangle(kPoint, kRectangle) == true);
  }
}
