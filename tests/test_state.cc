#include <catch2/catch.hpp>

#include "state.h"

using namespace turingmachinesimulator;

/**
 * Partitions Testing As Follows:
 * Correctly Checks If A Point Is Within a Given Radius The State
 */
TEST_CASE("Test Correctly Checks If A Point Is Within a Given Radius") {
  const glm::vec2 kStateLocation = glm::vec2(10, 10);
  const State kState = State(-1, "test_state", kStateLocation, 
      5);
  
  SECTION("Test Given Radius is Negative", "[within radius]") {
    const glm::vec2 kOtherPointLocation = glm::vec2(1, 1);
    REQUIRE(kState.StateCenterIsWithinGivenRadius(kOtherPointLocation, -10) 
        == false);
  }
  
  SECTION("Test Given Radius is 0 And Given Point Isn't State's Center", 
      "[within radius]") {
    const glm::vec2 kOtherPointLocation = glm::vec2(1, 1);
    REQUIRE(kState.StateCenterIsWithinGivenRadius(kOtherPointLocation, 0) 
        == false);
  }
  
  SECTION("Test Given Radius is 0 And Given Point Is State's Center",
      "[within radius]") {
    const glm::vec2 kOtherPointLocation = glm::vec2(10, 10);
    REQUIRE(kState.StateCenterIsWithinGivenRadius(kOtherPointLocation, 0));
  }
  
  SECTION("Test Given Point is Within the Given Radius", "[within radius]") {
    const glm::vec2 kOtherPointLocation = glm::vec2(13, 12);
    REQUIRE(kState.StateCenterIsWithinGivenRadius(kOtherPointLocation, 5));
  }
  SECTION("Test Given Point is Not Within the Given Radius", "[within radius]") {
    const glm::vec2 kOtherPointLocation = glm::vec2(14, 14);
    REQUIRE(kState.StateCenterIsWithinGivenRadius(kOtherPointLocation, 5) 
        == false);
  }
}