#include <catch2/catch.hpp>

#include "direction.h"

using namespace turingmachinesimulator;
/**
 * Partitions Test As Follows:
 * Direction Object Correctly Created
 */
TEST_CASE("Test Direction Object Is Created Correctly") {
  SECTION("Test Wrong Input String Vector Is Too Small", "[initialization]") {
    const std::vector<std::string> kInputs = {"a", "b", "l", "q1"};
    const std::vector<State> kPossibleStates = {State(1, "q1", 
        glm::vec2(0,0), 5)};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Wrong Input String Vector Is Too Large", "[initialization]") {
    const std::vector<std::string> kInputs = {"a", "b", "l", "q1", "qh",
        "random"};
    State kQ1 = State(1, "q1", 
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }
  
  SECTION("Test Possible State Vector Is Empty", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "b", "l", "q1", "qh"};
    const std::vector<State> kPossibleStates = {};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }
  
  SECTION("Test Read String Is Too Long", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"alphabet", "b", "l", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }
  
  SECTION("Test Read String Is Empty", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"", "b", "l", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Write String Is Too Long", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "alphabet", "l", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Write String Is Empty", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "", "l", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Shift String Is Too Long", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "b", "left", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Shift String Is Empty", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "b", "", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Shift String Contains Character Other Than L/R/N", 
      "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "b", "c", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }
      
  SECTION("Test Move From State Name Does Not Exist", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"alphabet", "b", "l", "qn", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
       glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Move To State Name Does Not Exist", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"alphabet", "b", "l", "q1", "qn"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }
  
  SECTION("Test Where Shift String Has Capital", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "b", "L", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty() == false);
    REQUIRE(kDirection.GetRead() == 'a');
    REQUIRE(kDirection.GetWrite() == 'b');
    REQUIRE(kDirection.GetScannerMovement() == 'l');
    REQUIRE(kDirection.GetStateToMoveFrom().Equals(kQ1));
    REQUIRE(kDirection.GetStateToMoveTo().Equals(kQh));
  }

  SECTION("Test Where Shift String Has Lowercase", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "b", "n", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty() == false);
    REQUIRE(kDirection.GetRead() == 'a');
    REQUIRE(kDirection.GetWrite() == 'b');
    REQUIRE(kDirection.GetScannerMovement() == 'n');
    REQUIRE(kDirection.GetStateToMoveFrom().Equals(kQ1));
    REQUIRE(kDirection.GetStateToMoveTo().Equals(kQh));
  }
}
