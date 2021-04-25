#include <catch2/catch.hpp>

#include "turing_machine.h"

using namespace turingmachinesimulator;

/**
 * Partitions testing as follows:
 * Constructor Properly Creates Turing Machine 
 */
TEST_CASE("Test Turing Machine Creation") {
  const State kStartingState = State(1, "q1", 
      glm::vec2(1, 1), 5);
  const State kStateTwo = State(2, "q2", 
      glm::vec2(0, 0), 6);
  const State kHaltingState = State(5, "qh", 
      glm::vec2(5, 6), 3);
  
  SECTION("Test No Starting State", "[initialization][empty][error]") {
    const std::vector<State> kStates = {kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStateTwo, kHaltingState);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections, 
        kTape);
    REQUIRE(kTuringMachine.IsEmpty());
    REQUIRE(kTuringMachine.GetErrorMessage() == 
        "Must Have Halting and Starting States");
  }
  
  SECTION("Test No Halting State", "[initialization][empty][error]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStartingState, kStateTwo);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections,
        kTape);
    REQUIRE(kTuringMachine.IsEmpty());
    REQUIRE(kTuringMachine.GetErrorMessage() ==
            "Must Have Halting and Starting States");
  }
  
  SECTION("Test No States", "[initialization][empty][error]") {
    const std::vector<State> kStates = {};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStartingState, kStateTwo);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections,
        kTape);
    REQUIRE(kTuringMachine.IsEmpty());
    REQUIRE(kTuringMachine.GetErrorMessage() ==
            "Must Have Halting and Starting States");
  }
  
  SECTION("Test States With Same Name", "[initialization][empty][error]") {
    const State kSecondStateTwo = State(3, "q2",
        glm::vec2(1, 1), 5);
    const std::vector<State> kStates = {kStartingState, kStateTwo, 
        kSecondStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStartingState, kStateTwo);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections,
        kTape);
    REQUIRE(kTuringMachine.IsEmpty());
    REQUIRE(kTuringMachine.GetErrorMessage() ==
        "Cannot Have States With The Same Name");
  }
  
  SECTION("Test 2 Directions From State With Same Read Condition",
      "[initialization][empty][error]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStartingState, kHaltingState);
    const Direction kDirectionTwo = Direction('0', '2', 'r',
        kStartingState, kStateTwo);
    const std::vector<Direction> kDirections = {kDirectionOne, kDirectionTwo};
    const std::vector<char> kTape = {};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections,
        kTape);
    REQUIRE(kTuringMachine.IsEmpty());
    REQUIRE(kTuringMachine.GetErrorMessage() ==
        "Must Not Have 2 Directions With Same Read Condition From The Same State");
  }
  
  SECTION("Test Correct Inputs", "[initialization][empty][direction map]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStartingState, kStateTwo);
    const Direction kDirectionTwo = Direction('1', '1', 'n',
        kStartingState, kHaltingState);
    const std::vector<Direction> kDirections = {kDirectionOne, kDirectionTwo};
    const std::vector<char> kTape = {};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections,
            kTape);
    // check turing machine is not empty and has no errors
    REQUIRE(kTuringMachine.IsEmpty() == false);
    REQUIRE(kTuringMachine.GetErrorMessage() == "");
    
    // check the tape is correctly initialized
    REQUIRE(kTuringMachine.GetTape() == kTape);
    
    // check starting state is correctly initialized
    REQUIRE(kTuringMachine.GetStartingState().Equals(kStartingState));
    const std::vector<State> kTuringMachineHaltingStates = 
        kTuringMachine.GetHaltingStates();
    
    // check halting states are correctly initialized
    REQUIRE(kTuringMachineHaltingStates.size() == 1);
    REQUIRE(kTuringMachineHaltingStates.at(0).Equals(kHaltingState));
    
    // check directions by states map is created correctly
    std::map<State, std::vector<Direction>> expected_directions_by_state_map;
    expected_directions_by_state_map[kStartingState] = kDirections;
    const std::map<State, std::vector<Direction>> kDirectionsByStateMap = 
        kTuringMachine.GetDirectionsByStateMap();
    // check map size
    REQUIRE(kDirectionsByStateMap.size() 
        == expected_directions_by_state_map.size());
    // check starting state Direction vector
    REQUIRE(kDirectionsByStateMap.at(kStartingState).size() ==
        expected_directions_by_state_map.at(kStartingState).size());
    REQUIRE(kDirectionsByStateMap.at(kStartingState).at(0).Equals
        (expected_directions_by_state_map.at(kStartingState).at(0)));
    REQUIRE(kDirectionsByStateMap.at(kStartingState).at(1).Equals
        (expected_directions_by_state_map.at(kStartingState).at(1)));
  }
  
  SECTION("Test Multiple Halting States", "[initialization][empty]"
      "[direction map][halting states]") {
    const State kSecondHaltingState = State(7, "qh", 
        glm::vec2(5, 6), 9);
    const std::vector<State> kStates = {kStartingState, kStateTwo,
        kHaltingState, kSecondHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStartingState, kStateTwo);
    const Direction kDirectionTwo = Direction('1', '1', 'n',
        kStateTwo, kStartingState);
    const std::vector<Direction> kDirections = {kDirectionOne, kDirectionTwo};
    const std::vector<char> kTape = {1, 0, 1, 1};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections,
        kTape);
    // check turing machine is not empty and has no errors
    REQUIRE(kTuringMachine.IsEmpty() == false);
    REQUIRE(kTuringMachine.GetErrorMessage() == "");

    // check the tape is correctly initialized
    REQUIRE(kTuringMachine.GetTape() == kTape);

    // check starting state is correctly initialized
    REQUIRE(kTuringMachine.GetStartingState().Equals(kStartingState));
    const std::vector<State> kTuringMachineHaltingStates =
        kTuringMachine.GetHaltingStates();

    // check halting states are correctly initialized
    REQUIRE(kTuringMachineHaltingStates.size() == 2);
    REQUIRE(kTuringMachineHaltingStates.at(0).Equals(kHaltingState));
    REQUIRE(kTuringMachineHaltingStates.at(1).Equals(kSecondHaltingState));
    
    // check directions by states map is created correctly
    std::map<State, std::vector<Direction>> expected_directions_by_state_map;
    expected_directions_by_state_map[kStartingState] = {kDirectionOne};
    expected_directions_by_state_map[kStateTwo] = {kDirectionTwo};
    const std::map<State, std::vector<Direction>> kDirectionsByStateMap =
        kTuringMachine.GetDirectionsByStateMap();
    // check map size
    REQUIRE(kDirectionsByStateMap.size()
        == expected_directions_by_state_map.size());
    // check starting state Direction vector
    REQUIRE(kDirectionsByStateMap.at(kStartingState).size() ==
        expected_directions_by_state_map.at(kStartingState).size());
    REQUIRE(kDirectionsByStateMap.at(kStartingState).at(0).Equals
        (expected_directions_by_state_map.at(kStartingState).at(0)));
    // check state two Direction Vector
    REQUIRE(kDirectionsByStateMap.at(kStateTwo).size() 
        == expected_directions_by_state_map.at(kStateTwo).size());
    REQUIRE(kDirectionsByStateMap.at(kStateTwo).at(0).Equals
        (expected_directions_by_state_map.at(kStateTwo).at(0)));
  }
}
