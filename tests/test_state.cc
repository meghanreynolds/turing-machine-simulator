#include <catch2/catch.hpp>

#include "state.h"

using namespace turingmachinesimulator;

/**
 * Partitions Testing As Follows:
 * Correctly Marks State Objects As Empty/Non-empty
 * Correctly Updates Position
 * Correctly Checks If A Point Is Within a Given Radius The State
 * Correctly Checks For Equality With Other States
 * Less Than Operator Functions Correctly
 */
TEST_CASE("Test Correctly Marks Objects As Empty/Nonempty") {
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  
  SECTION("Test Object Made With Default Constructor", "[initialization]"
      "[empty]") {
    const State kState = State(); 
    REQUIRE(kState.IsEmpty());
  }
  
  SECTION("Test Object Made Without Default Constructor", "[initialization]"
      "[empty]") {
    const State kState = State(1, "q1", 
        glm::vec2(0, 0), 4, kHaltingStateNames);
    REQUIRE(kState.IsEmpty() == false);
  }
}

TEST_CASE("Test Correctly Updates Position") {
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  
  SECTION("Test Position Is The Same", "[update position]") {
    State state = State(1, "q1",
        glm::vec2(0, 0), 4, kHaltingStateNames);
    state.SetStateLocation(glm::vec2(0, 0));
    REQUIRE(state.GetStateLocation() == glm::vec2(0, 0));
  }
  
  SECTION("Test Position Is Different", "[update position]") {
    State state = State(1, "q1",
        glm::vec2(0, 0), 4, kHaltingStateNames);
    state.SetStateLocation(glm::vec2(10, 9));
    REQUIRE(state.GetStateLocation() == glm::vec2(10, 9));
  }
}

TEST_CASE("Test Correctly Checks If A Point Is Within a Given Radius") {
  const glm::vec2 kStateLocation = glm::vec2(10, 10);
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  const State kState = State(-1, "test_state", kStateLocation, 
      5, kHaltingStateNames);
  
  SECTION("Test Given Radius is Negative", "[within radius]") {
    const glm::vec2 kOtherPointLocation = glm::vec2(1, 1);
    REQUIRE(kState.IsStateCenterWithinGivenRadiusOfPoint(kOtherPointLocation, 
        -10) == false);
  }
  
  SECTION("Test Given Radius is 0 And Given Point Isn't State's Center", 
      "[within radius]") {
    const glm::vec2 kOtherPointLocation = glm::vec2(1, 1);
    REQUIRE(kState.IsStateCenterWithinGivenRadiusOfPoint(kOtherPointLocation, 0) 
        == false);
  }
  
  SECTION("Test Given Radius is 0 And Given Point Is State's Center",
      "[within radius]") {
    const glm::vec2 kOtherPointLocation = glm::vec2(10, 10);
    REQUIRE(kState.IsStateCenterWithinGivenRadiusOfPoint(kOtherPointLocation, 0));
  }
  
  SECTION("Test Given Point is Within the Given Radius", "[within radius]") {
    const glm::vec2 kOtherPointLocation = glm::vec2(13, 12);
    REQUIRE(kState.IsStateCenterWithinGivenRadiusOfPoint(kOtherPointLocation, 5));
  }
  SECTION("Test Given Point is Not Within the Given Radius", "[within radius]") {
    const glm::vec2 kOtherPointLocation = glm::vec2(14, 14);
    REQUIRE(kState.IsStateCenterWithinGivenRadiusOfPoint(kOtherPointLocation, 5) 
        == false);
  }
}

TEST_CASE("Test Correctly Evaluates Equality With Other States") {
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  
  SECTION("Test 2 States Have No Similar Values", "[equality]") {
    const State kState1 = State(1, "q1", 
        glm::vec2(1, 1), 5, {});
    const State kState2 = State(2, "qh", 
        glm::vec2(0, 0), 2, kHaltingStateNames);
    REQUIRE(kState1.Equals(kState2) == false);
  }
  
  SECTION("Test 2 States Have All Values The Same Except Id's", "[equality]") {
    const State kState1 = State(1, "q1",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const State kState2 = State(2, "q1",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    REQUIRE(kState1.Equals(kState2) == false);
  }
  
  SECTION("Test 2 States Have Different Values But Same Id's", "[equality]") {
    const State kState1 = State(1, "q1",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const State kState2 = State(1, "qh",
        glm::vec2(0, 0), 2, {});
    REQUIRE(kState1.Equals(kState2));
  }
  
  SECTION("Test 2 States Have All Values The Same", "[equality]") {
    const State kState1 = State(1, "q1",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const State kState2 = State(1, "q1",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    REQUIRE(kState1.Equals(kState2));
  }
}

TEST_CASE("Test Less Than Operator") {
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  
  SECTION("Test Id Greater Than Other Id", "[less than]") {
    const State kStateToCompare = State(1, "q1", 
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const State kStateWithSmallerId = State(0, "q0", 
        glm::vec2(1, 1), 5, kHaltingStateNames);
    REQUIRE((kStateToCompare < kStateWithSmallerId) == false);
  }
  
  SECTION("Test Equal Id's", "[less than]") {
    const State kStateToCompare = State(1, "q1",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const State kStateWithSmallerId = State(1, "q0",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    REQUIRE((kStateToCompare < kStateWithSmallerId) == false);
  }
  
  SECTION("Test Id Less Than Other Id", "[less than]") {
    const State kStateToCompare = State(0, "q0",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const State kStateWithSmallerId = State(1, "q1",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    REQUIRE(kStateToCompare < kStateWithSmallerId);
  }
}
