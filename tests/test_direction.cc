#include <catch2/catch.hpp>

#include "direction.h"

using namespace turingmachinesimulator;

/**
 * Partitions Test As Follows:
 * Direction Object Correctly Created From Vectors
 * Direction Object Correctly Created From Given Characters and States
 * String Representation Correctly Created 
 * Equality Operator Correctly Overloaded
 * Direction Objects Are Correctly Compared For Equality
 */
TEST_CASE("Test Direction Object Is Created Correctly From Vectors") {
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept", 
      "qReject"};
  SECTION("Test Wrong Input String Vector Is Too Small", "[initialization]") {
    const std::vector<std::string> kInputs = {"a", "b", "l", "q1"};
    const std::vector<State> kPossibleStates = {State(1, "q1", 
        glm::vec2(0,0), 5, kHaltingStateNames)};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Wrong Input String Vector Is Too Large", "[initialization]") {
    const std::vector<std::string> kInputs = {"a", "b", "l", "q1", "qh",
        "random"};
    State kQ1 = State(1, "q1", 
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
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
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }
  
  SECTION("Test Read String Is Empty", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"", "b", "l", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Write String Is Too Long", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "alphabet", "l", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Write String Is Empty", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "", "l", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Shift String Is Too Long", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "b", "left", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Shift String Is Empty", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "b", "", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Shift String Contains Character Other Than L/R/N", 
      "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "b", "c", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }
      
  SECTION("Test Move From State Name Does Not Exist", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "b", "l", "qn", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
       glm::vec2(0, 1), 6, kHaltingStateNames);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test Move To State Name Does Not Exist", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"alphabet", "b", "l", "q1", "qn"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.IsEmpty());
  }
  
  SECTION("Test Where Shift String Has Capital", "[initialization][empty]") {
    const std::vector<std::string> kInputs = {"a", "b", "L", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
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
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
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

TEST_CASE("Test Direction Is Correctly Created From Chars And States") {
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  
  SECTION("Test Scanner Movement Char Is Not l/r/n", "[initialization]") {
    const State kState = State(1, "q1", 
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const State kStateTwo = State(2, "q2",
        glm::vec2(1, 2), 5, kHaltingStateNames);
    const Direction kDirection = Direction('a', 'b', 'c', 
        kState, kStateTwo);
    REQUIRE(kDirection.IsEmpty());
  }

  SECTION("Test State To Move From Is Empty", "[initialization]") {
    const State kState = State();
    const State kStateTwo = State(1, "q1",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const Direction kDirection = Direction('a', 'b', 'l',
        kState, kStateTwo);
    REQUIRE(kDirection.IsEmpty());
  }
  
  SECTION("Test State To Move To Is Empty", "[initialization]") {
    const State kState = State(1, "q1",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const State kStateTwo = State();
    const Direction kDirection = Direction('a', 'b', 'l',
        kState, kStateTwo);
    REQUIRE(kDirection.IsEmpty());
  }
  
  SECTION("Test All Information Is Correct", "[initialization]") {
    const State kState = State(2, "q2",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const State kStateTwo = State(1, "q1",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const Direction kDirection = Direction('a', 'b', 'l',
        kState, kStateTwo);
    REQUIRE(kDirection.GetRead() == 'a');
    REQUIRE(kDirection.GetWrite() == 'b');
    REQUIRE(kDirection.GetScannerMovement() == 'l');
    REQUIRE(kDirection.GetStateToMoveFrom().Equals(kState));
    REQUIRE(kDirection.GetStateToMoveTo().Equals(kStateTwo));
  }

  SECTION("Test With Capital Scanner Movement Char", "[initialization]") {
    const State kState = State(2, "q2",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const State kStateTwo = State(1, "q1",
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const Direction kDirection = Direction('a', 'b', 'R',
        kState, kStateTwo);
    REQUIRE(kDirection.GetRead() == 'a');
    REQUIRE(kDirection.GetWrite() == 'b');
    REQUIRE(kDirection.GetScannerMovement() == 'r');
    REQUIRE(kDirection.GetStateToMoveFrom().Equals(kState));
    REQUIRE(kDirection.GetStateToMoveTo().Equals(kStateTwo));
  }
}

TEST_CASE("Test String Representation Correctly Created") {
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  
  SECTION("Test String Representation", "[string]") {
    const std::vector<std::string> kInputs = {"a", "b", "n", "q1", "qh"};
    State kQ1 = State(1, "q1",
        glm::vec2(0, 0), 5, kHaltingStateNames);
    State kQh = State(2, "qh",
        glm::vec2(0, 1), 6, kHaltingStateNames);
    const std::vector<State> kPossibleStates = {kQ1, kQh};
    const Direction kDirection = Direction(kInputs, kPossibleStates);
    REQUIRE(kDirection.ToString() == "a, b, N");
  }
}

TEST_CASE("Test Equality Operator") {
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  const State kQ1 = State(1, "q1",
      glm::vec2(0, 0), 5, kHaltingStateNames);
  const State kQ2 = State(2, "q2",
      glm::vec2(1, 1), 7, kHaltingStateNames);
  const State kQ3 = State(3, "q3",
      glm::vec2(2, 2), 8, kHaltingStateNames);
  const State kQh = State(4, "qh",
      glm::vec2(0, 1), 6, kHaltingStateNames);
  
  SECTION("Test Directions With Nothing In Common", "[equality]") {
    const Direction kDirectionOne = Direction('a', 'b', 'n', 
        kQ1, kQh);
    const Direction kDirectionTwo = Direction('c', 'd', 'l',
        kQ2, kQ3);
    REQUIRE((kDirectionOne == kDirectionTwo) == false);
  }
  
  SECTION("Test Empty Direction With Non-Empty Direction", "[equality]") {
    const Direction kDirection = Direction();
    const Direction kDirectionTwo = Direction('b', 'c', 'r',
        kQ1, kQ2);
    REQUIRE((kDirection == kDirectionTwo) == false);
  }
  
  SECTION("Test Directions With Everything Except Read Character The Same", 
       "[equality]") {
    const Direction kDirectionOne = Direction('a', 'b', 'n', 
        kQ1, kQh);
    const Direction kDirectionTwo = Direction('c', 'b', 'n', 
        kQ1, kQh);
    REQUIRE((kDirectionOne == kDirectionTwo) == false);
  }
  
  SECTION("Test Directions With Everything The Same", "[equality]") {
    const Direction kDirectionOne = Direction('a', 'b', 'n', 
        kQ1, kQh);
    const Direction kDirectionTwo = Direction('a', 'b', 'N', 
        kQ1, kQh);
    REQUIRE(kDirectionOne == kDirectionTwo);
  }
  
  SECTION("Test Directions With Everything Except Read Character Different",
      "[equality]") {
    const Direction kDirectionOne = Direction('a', 'b', 'n', 
        kQ1, kQh);
    const Direction kDirectionTwo = Direction('a', 'd', 'l', 
        kQ2, kQ3);
    REQUIRE(kDirectionOne == kDirectionTwo);
  }

  SECTION("Test 2 Empty Directions", "[equality]") {
    const Direction kDirection = Direction();
    const Direction kDirectionTwo = Direction();
    REQUIRE(kDirection == kDirectionTwo);
  }
}

TEST_CASE("Test Equality With Other Directions") {
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  const State kStateOne = State(1, "q1", 
      glm::vec2(0, 5), 5, kHaltingStateNames);
  const State kStateTwo= State(2, "q2", 
      glm::vec2(0, 5), 5, kHaltingStateNames);
  
  SECTION("Test Directions With All Different Variables", "[equality]") {
    const Direction kDirection = Direction('a', 'c', 'l', 
        kStateOne, kStateTwo);
    const Direction kDirectionTwo = Direction('b', 'c', 'r',
        kStateTwo, kStateOne);
    REQUIRE(kDirection.Equals(kDirectionTwo) == false);
  }
  
  SECTION("Test Empty Direction With Non-Empty Direction", "[equality]") {
    const Direction kDirection = Direction('a', 'c', 'l',
        kStateOne, kStateTwo);
    const Direction kDirectionTwo = Direction();
    REQUIRE(kDirection.Equals(kDirectionTwo) == false);
  }
  
  SECTION("Test Directions With Only Same Read Characters", "[equality]") {
    const Direction kDirection = Direction('a', 'c', 'l',
        kStateOne, kStateTwo);
    const Direction kDirectionTwo = Direction('a', 'c', 'r',
        kStateTwo, kStateOne);
    REQUIRE(kDirection.Equals(kDirectionTwo) == false);
  }
  
  SECTION("Test Directions With All Variables The Same", "[equality]") {
    const Direction kDirection = Direction('a', 'b', 'l',
        kStateOne, kStateTwo);
    const Direction kDirectionTwo = Direction('a', 'b', 'l',
        kStateOne, kStateTwo);
    REQUIRE(kDirection.Equals(kDirectionTwo));
  }

  SECTION("Test Two Empty Directions", "[equality]") {
    const Direction kDirection = Direction();
    const Direction kDirectionTwo = Direction();
    REQUIRE(kDirection.Equals(kDirectionTwo));
  }
}
