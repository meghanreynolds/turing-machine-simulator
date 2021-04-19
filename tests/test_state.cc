#include <catch2/catch.hpp>

#include "state.h"

using namespace turingmachinesimulator;

/**
 * Partitions Testing As Follows:
 * Correctly Marks Objects As Empty/Non-empty
 * Correctly Checks If A Point Is Within a Given Radius The State
 * Correctly Checks For Equality With Other States
 */
TEST_CASE("Test Correctly Marks Objects As Empty/Nonempty") {
  SECTION("Test Object Made With Default Constructor", "[initialization]"
      "[empty]") {
    const State kState = State(); 
    REQUIRE(kState.IsEmpty());
  }
  
  SECTION("Test Object Made Without Default Constructor", "[initialization]"
      "[empty]") {
    const State kState = State(1, "q1", 
        glm::vec2(0, 0), 4);
    REQUIRE(kState.IsEmpty() == false);
  }
}

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

TEST_CASE("Test Correctly Evaluates Equality With Other States") {
  SECTION("Test 2 States Have No Similar Values", "[equality]") {
    const State kState1 = State(1, "q1", 
        glm::vec2(1, 1), 5);
    const State kState2 = State(2, "qh", 
        glm::vec2(0, 0), 2);
    REQUIRE(kState1.Equals(kState2) == false);
  }
  
  SECTION("Test 2 States Have All Values The Same Except Id's", "[equality]") {
    const State kState1 = State(1, "q1",
        glm::vec2(1, 1), 5);
    const State kState2 = State(2, "q1",
        glm::vec2(1, 1), 5);
    REQUIRE(kState1.Equals(kState2) == false);
  }
  
  SECTION("Test 2 States Have Different Values But Same Id's", "[equality]") {
    const State kState1 = State(1, "q1",
        glm::vec2(1, 1), 5);
    const State kState2 = State(1, "qh",
        glm::vec2(0, 0), 2);
    REQUIRE(kState1.Equals(kState2));
  }
  
  SECTION("Test 2 States Have All Values The Same", "[equality]") {
    const State kState1 = State(1, "q1",
        glm::vec2(1, 1), 5);
    const State kState2 = State(1, "q1",
        glm::vec2(1, 1), 5);
    REQUIRE(kState1.Equals(kState2));
  }
}
