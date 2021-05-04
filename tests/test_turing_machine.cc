#include <catch2/catch.hpp>

#include "turing_machine.h"

using namespace turingmachinesimulator;

/**
 * Partitions testing as follows:
 * Constructor Properly Creates Turing Machine 
 * Turing Machine Correctly Updates
 * Configuration Is Produced Correctly For The Console
 * Configuration Is Produced Correctly For Markdown Files
 */
TEST_CASE("Test Turing Machine Creation") {
  const char kBlankChar = '0';
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  const State kStartingState = State(1, "q1", 
      glm::vec2(1, 1), 5, kHaltingStateNames);
  const State kStateTwo = State(2, "q2", 
      glm::vec2(0, 0), 6, kHaltingStateNames);
  const State kHaltingState = State(5, "qh", 
      glm::vec2(5, 6), 3, kHaltingStateNames);
  
  SECTION("Test No Starting State", "[initialization][empty][error]") {
    const std::vector<State> kStates = {kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStateTwo, kHaltingState);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections, 
        kTape, kBlankChar, kHaltingStateNames);
    REQUIRE(kTuringMachine.IsEmpty());
    REQUIRE(kTuringMachine.GetErrorMessage() == "Must Have Starting State");
  }
  
  SECTION("Test Multiple Starting States", "[initialization][empty][error]") {
    const State kSecondStartState = State(2, "q1", 
        glm::vec2(9, 10), 6, kHaltingStateNames);
    const std::vector<State> kStates = {kStartingState, kSecondStartState};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStartingState, kStartingState);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    REQUIRE(kTuringMachine.IsEmpty());
    REQUIRE(kTuringMachine.GetErrorMessage()
        == "Cannot Have More Than 1 Starting State");
  }
  
  SECTION("Test No States", "[initialization][empty][error]") {
    const std::vector<State> kStates = {};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStartingState, kStateTwo);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    REQUIRE(kTuringMachine.IsEmpty());
    REQUIRE(kTuringMachine.GetErrorMessage() == "Must Have Starting State");
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
        kTape, kBlankChar, kHaltingStateNames);
    REQUIRE(kTuringMachine.IsEmpty());
    REQUIRE(kTuringMachine.GetErrorMessage() ==
        "Must Not Have 2 Directions With Same Read Condition From The Same "
        "State");
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
        kTape, kBlankChar, kHaltingStateNames);
    
    // check turing machine is not empty and has no errors
    REQUIRE(kTuringMachine.IsEmpty() == false);
    REQUIRE(kTuringMachine.GetErrorMessage().empty());
    
    // check the tape is correctly initialized
    REQUIRE(kTuringMachine.GetTape() == std::vector<char>({'-'}));

    // check that the current state is correctly initialized
    REQUIRE(kTuringMachine.GetCurrentState().Equals(kStartingState));
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
    
    // check not halted
    REQUIRE(kTuringMachine.IsHalted() == false);
    
    // check scanner starts at index 0
    REQUIRE(kTuringMachine.GetIndexOfScanner() == 0);
  }
  
  SECTION("Test Multiple Halting States", "[initialization][empty]"
      "[direction map][halting states]") {
    const State kSecondHaltingState = State(7, "qAccept", 
        glm::vec2(5, 6), 9, kHaltingStateNames);
    const State kThirdHaltingState = State(10, "qReject", 
        glm::vec2(17, 9), 5, kHaltingStateNames);
    const std::vector<State> kStates = {kStartingState, kStateTwo,
        kHaltingState, kSecondHaltingState, kThirdHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStartingState, kStateTwo);
    const Direction kDirectionTwo = Direction('1', '1', 'n',
        kStateTwo, kStartingState);
    const std::vector<Direction> kDirections = {kDirectionOne, kDirectionTwo};
    const std::vector<char> kTape = {1, 0, 1, 1};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    
    // check turing machine is not empty and has no errors
    REQUIRE(kTuringMachine.IsEmpty() == false);
    REQUIRE(kTuringMachine.GetErrorMessage() == "");

    // check the tape is correctly initialized
    REQUIRE(kTuringMachine.GetTape() == kTape);

    // check that the current state is correctly initialized
    REQUIRE(kTuringMachine.GetCurrentState().Equals(kStartingState));
    const std::vector<State> kTuringMachineHaltingStates =
        kTuringMachine.GetHaltingStates();

    // check halting states are correctly initialized
    REQUIRE(kTuringMachineHaltingStates.size() == 3);
    REQUIRE(kTuringMachineHaltingStates.at(0).Equals(kHaltingState));
    REQUIRE(kTuringMachineHaltingStates.at(1).Equals(kSecondHaltingState));
    REQUIRE(kTuringMachineHaltingStates.at(2).Equals(kThirdHaltingState));
    
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

    // check not halted
    REQUIRE(kTuringMachine.IsHalted() == false);

    // check scanner starts at index 0
    REQUIRE(kTuringMachine.GetIndexOfScanner() == 0);
  }
  
  SECTION("Test Turing Machine Without Directions", "[initialization][empty]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const std::vector<Direction> kDirections = {};
    const std::vector<char> kTape = {'-'};
    const TuringMachine kTuringMachine = TuringMachine(kStates, kDirections, 
        kTape, kBlankChar, kHaltingStateNames);
    
    // check that the turing machine isn't empty and that it doesn't have errors
    REQUIRE(kTuringMachine.IsEmpty() == false);
    REQUIRE(kTuringMachine.GetErrorMessage() == "");
    
    // check that the current state is correctly initialized
    REQUIRE(kTuringMachine.GetCurrentState().Equals(kStartingState));
    
    // check that the tape is correctly initialized
    REQUIRE(kTuringMachine.GetTape() == kTape);
    
    // check that the halting states are correctly initialized
    const std::vector<State> kHaltingStates = kTuringMachine.GetHaltingStates();
    REQUIRE(kHaltingStates.size() == 1);
    REQUIRE(kHaltingStates.at(0).Equals(kHaltingState));
    
    // check that the directions by state map is empty
    REQUIRE(kTuringMachine.GetDirectionsByStateMap().empty());
  }
}

TEST_CASE("Test Turing Machine Correctly Updates") {
  const char kBlankChar = '-';
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  const State kStartingState = State(1, "q1",
      glm::vec2(1, 1), 5, kHaltingStateNames);
  const State kStateTwo = State(2, "q2",
      glm::vec2(0, 0), 6, kHaltingStateNames);
  const State kHaltingState = State(5, "qReject",
      glm::vec2(5, 6), 3, kHaltingStateNames);
  
  SECTION("Test Turing Machine Without Any Directions", "[update]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const std::vector<Direction> kDirections = {};
    const std::vector<char> kTape = {'-'};
    TuringMachine turing_machine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == false);
    REQUIRE(turing_machine.GetTape() == kTape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 0);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStartingState));
  }
  
  SECTION("Test Turing Machine Where Directions Don't Apply", "[update]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('a', 'b', 'l', 
        kStartingState, kHaltingState);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {'b', 'c', 'd'};
    TuringMachine turing_machine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == false);
    REQUIRE(turing_machine.GetTape() == kTape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 0);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStartingState));
  }

  SECTION("Test Turing Machine Correctly Follows Left Movement", "[update]"
      "[left][right]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'r',
        kStartingState, kStateTwo);
    const Direction kDirectionTwo = Direction('-', '1', 'L', 
        kStateTwo, kStartingState);
    const std::vector<Direction> kDirections = {kDirectionOne, kDirectionTwo};
    const std::vector<char> kTape = {'0', '-'};
    TuringMachine turing_machine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    // need to do 2 updates for left movement to occur
    turing_machine.Update();
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == false);
    const std::vector<char> kExpectedTape = {'1', '1'};
    REQUIRE(turing_machine.GetTape() == kExpectedTape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 0);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStartingState));
  }
  
  SECTION("Test Turing Machine Correctly Follows Right Movement", "[update]"
      "[right]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'r',
        kStartingState, kStateTwo);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {'0', '-'};
    TuringMachine turing_machine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == false);
    const std::vector<char> kExpectedTape = {'1', '-'};
    REQUIRE(turing_machine.GetTape() == kExpectedTape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 1);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStateTwo));
  }

  SECTION("Test Turing Machine Correctly Follows No Movement", "[update][no]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'n',
        kStartingState, kStateTwo);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {'0', '-'};
    TuringMachine turing_machine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == false);
    const std::vector<char> kExpectedTape = {'1', '-'};
    REQUIRE(turing_machine.GetTape() == kExpectedTape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 0);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStateTwo));
  }
  
  SECTION("Test Turing Machine Halts Correctly", "[update][right][halt]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'r',
        kStartingState, kHaltingState);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {'0', '-'};
    TuringMachine turing_machine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == true);
    const std::vector<char> kExpectedTape = {'1', '-'};
    REQUIRE(turing_machine.GetTape() == kExpectedTape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 1);
    REQUIRE(turing_machine.GetCurrentState().Equals(kHaltingState));
  }
  
  SECTION("Test Turing Machine Where Tape Needs To Expand At Front", "[update]"
      "[tape expansion][left]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'l',
        kStartingState, kStateTwo);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {'0', '-'};
    TuringMachine turing_machine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    turing_machine.Update();
    
    // test machine halts and updates properly
    REQUIRE(turing_machine.IsHalted() == false);
    const std::vector<char> kExpectedTape = {'-','1', '-'};
    REQUIRE(turing_machine.GetTape() == kExpectedTape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 0);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStateTwo));
    
    // test machine does not do anything after halting
    REQUIRE(turing_machine.IsHalted() == false);
    REQUIRE(turing_machine.GetTape() == kExpectedTape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 0);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStateTwo));
  }
  
  SECTION("Test Turing Machine Where Tape Needs To Expand At End", "[update]"
      "[right][tape expansion]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'r',
        kStartingState, kStateTwo);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {'0'};
    TuringMachine turing_machine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == false);
    const std::vector<char> kExpectedTape = {'1', '-'};
    REQUIRE(turing_machine.GetTape() == kExpectedTape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 1);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStateTwo));
  }
  
  SECTION("Test Turing Machine With Self Loop", "[update][self loop][right]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'r',
        kStartingState, kStartingState);
    const std::vector<Direction> kDirections = {kDirectionOne};
    const std::vector<char> kTape = {'0', '-'};
    TuringMachine turing_machine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == false);
    const std::vector<char> kExpectedTape = {'1', '-'};
    REQUIRE(turing_machine.GetTape() == kExpectedTape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 1);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStartingState));
  }
  
  SECTION("Test Turing Machine Correctly Executes A Series Of Updates", 
      "[update][tape expansion][left][right][no][halt][self loop]") {
    const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'l',
        kStartingState, kStateTwo);
    const Direction kDirectionTwo = Direction('1', '1', 'r',
        kStartingState, kStartingState);
    const Direction kDirectionThree = Direction('-', '1', 'r',
        kStartingState, kHaltingState);
    const Direction kDirectionFour= Direction('-', '1', 'n',
        kStateTwo, kStartingState);
    const std::vector<Direction> kDirections = {kDirectionOne, kDirectionTwo, 
        kDirectionThree, kDirectionFour};
    const std::vector<char> kTape = {'0', '-'};
    TuringMachine turing_machine = TuringMachine(kStates, kDirections,
        kTape, kBlankChar, kHaltingStateNames);
    
    // test first update
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == false);
    std::vector<char> expected_tape = {'-','1', '-'};
    REQUIRE(turing_machine.GetTape() == expected_tape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 0);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStateTwo));
    
    // test second update
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == false);
    expected_tape = {'1', '1', '-'};
    REQUIRE(turing_machine.GetTape() == expected_tape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 0);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStartingState));
    
    // test third update
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == false);
    expected_tape = {'1', '1', '-'};
    REQUIRE(turing_machine.GetTape() == expected_tape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 1);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStartingState));
    
    // test fourth update
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == false);
    expected_tape = {'1', '1', '-'};
    REQUIRE(turing_machine.GetTape() == expected_tape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 2);
    REQUIRE(turing_machine.GetCurrentState().Equals(kStartingState));
    
    // test fifth update
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == true);
    expected_tape = {'1', '1', '1', '-'};
    REQUIRE(turing_machine.GetTape() == expected_tape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 3);
    REQUIRE(turing_machine.GetCurrentState().Equals(kHaltingState));
    
    // check machine doesn't do anything after halting
    turing_machine.Update();
    REQUIRE(turing_machine.IsHalted() == true);
    REQUIRE(turing_machine.GetTape() == expected_tape);
    REQUIRE(turing_machine.GetIndexOfScanner() == 3);
    REQUIRE(turing_machine.GetCurrentState().Equals(kHaltingState));
  }
}

TEST_CASE("Test Configuration Output For Console") {
  const char kBlankChar = '-';
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  const State kStartingState = State(1, "q1",
      glm::vec2(1, 1), 5, kHaltingStateNames);
  const State kStateTwo = State(2, "q2",
      glm::vec2(0, 0), 6, kHaltingStateNames);
  const State kHaltingState = State(5, "qAccept",
      glm::vec2(5, 6), 3, kHaltingStateNames);
  const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
  const Direction kDirectionOne = Direction('0', '1', 'l',
      kStartingState, kStateTwo);
  const Direction kDirectionTwo = Direction('1', '1', 'r',
      kStartingState, kStartingState);
  const Direction kDirectionThree = Direction('-', '1', 'r',
      kStartingState, kHaltingState);
  const Direction kDirectionFour= Direction('-', '1', 'n',
      kStateTwo, kStartingState);
  const std::vector<Direction> kDirections = {kDirectionOne, kDirectionTwo,
      kDirectionThree, kDirectionFour};
  const std::vector<char> kTape = {'0', '-'};
  TuringMachine turing_machine = TuringMachine(kStates, kDirections,
      kTape, kBlankChar, kHaltingStateNames);
  
  SECTION("Test Initial Position", "[console configuration]") {
    const std::string kExpectedConfiguration = ";q10-";
    REQUIRE(turing_machine.GetConfigurationForConsole() 
        == kExpectedConfiguration);
  }
  
  SECTION("Test Configuration Updates Correctly After Front Tape Expansions", 
      "[console configuration]") {
    turing_machine.Update();
    const std::string kExpectedConfiguration = ";q2-1-";
    REQUIRE(turing_machine.GetConfigurationForConsole() 
        == kExpectedConfiguration);
  }
  
  SECTION("Test Configuration Updates Correctly With The Machine", 
      "[console configuration]") {
    // test second update
    turing_machine.Update();
    turing_machine.Update();
    std::string expected_configuration = ";q111-";
    REQUIRE(turing_machine.GetConfigurationForConsole() 
        == expected_configuration);
    
    // test third update
    turing_machine.Update();
    expected_configuration = ";1q11-";
    REQUIRE(turing_machine.GetConfigurationForConsole() 
        == expected_configuration);
    
    // test fourth update
    turing_machine.Update();
    expected_configuration = ";11q1-";
    REQUIRE(turing_machine.GetConfigurationForConsole() 
        == expected_configuration);
  }
  
  SECTION("Test Configuration Updates Correctly After End Tape Expansions",
      "[console configuration]") {
    // test fifth update (where end tape expansion occurs)
    turing_machine.Update();
    turing_machine.Update();
    turing_machine.Update();
    turing_machine.Update();
    turing_machine.Update();
    const std::string kExpectedConfiguration = ";111qAccept-";
    REQUIRE(turing_machine.GetConfigurationForConsole() 
        == kExpectedConfiguration);
  }
}

TEST_CASE("Test Configuration Output For Markdown Files") {
  const char kBlankChar = '*';
  const std::vector<std::string> kHaltingStateNames = {"qh", "qAccept",
      "qReject"};
  const State kStartingState = State(1, "q1",
      glm::vec2(1, 1), 5, kHaltingStateNames);
  const State kStateTwo = State(2, "q2",
      glm::vec2(0, 0), 6, kHaltingStateNames);
  const State kHaltingState = State(5, "qh",
      glm::vec2(5, 6), 3, kHaltingStateNames);
  const std::vector<State> kStates = {kStartingState, kStateTwo, kHaltingState};
  const Direction kDirectionOne = Direction('0', '1', 'l',
      kStartingState, kStateTwo);
  const Direction kDirectionTwo = Direction('1', '1', 'r',
      kStartingState, kStartingState);
  const Direction kDirectionThree = Direction('*', '1', 'r',
      kStartingState, kHaltingState);
  const Direction kDirectionFour= Direction('*', '1', 'n',
      kStateTwo, kStartingState);
  const std::vector<Direction> kDirections = {kDirectionOne, kDirectionTwo,
      kDirectionThree, kDirectionFour};
  const std::vector<char> kTape = {'0', '*'};
  TuringMachine turing_machine = TuringMachine(kStates, kDirections,
      kTape, kBlankChar, kHaltingStateNames);

  SECTION("Test Initial Position", "[markdown configuration]") {
    const std::string kExpectedConfiguration = ";q<sub>1</sub>0*";
    REQUIRE(turing_machine.GetConfigurationForMarkdown()
        == kExpectedConfiguration);
  }

  SECTION("Test Configuration Updates Correctly After Front Tape Expansions",
      "[markdown configuration]") {
    turing_machine.Update();
    const std::string kExpectedConfiguration = ";q<sub>2</sub>*1*";
    REQUIRE(turing_machine.GetConfigurationForMarkdown()
        == kExpectedConfiguration);
  }

  SECTION("Test Configuration Updates Correctly With The Machine",
      "[markdown configuration]") {
    // test second update
    turing_machine.Update();
    turing_machine.Update();
    std::string expected_configuration = ";q<sub>1</sub>11*";
    REQUIRE(turing_machine.GetConfigurationForMarkdown()
        == expected_configuration);

    // test third update
    turing_machine.Update();
    expected_configuration = ";1q<sub>1</sub>1*";
    REQUIRE(turing_machine.GetConfigurationForMarkdown()
        == expected_configuration);

    // test fourth update
    turing_machine.Update();
    expected_configuration = ";11q<sub>1</sub>*";
    REQUIRE(turing_machine.GetConfigurationForMarkdown()
        == expected_configuration);
  }

  SECTION("Test Configuration Updates Correctly After End Tape Expansions",
      "[markdown configuration]") {
    // test fifth update (where end tape expansion occurs)
    turing_machine.Update();
    turing_machine.Update();
    turing_machine.Update();
    turing_machine.Update();
    turing_machine.Update();
    const std::string kExpectedConfiguration = ";111q<sub>h</sub>*";
    REQUIRE(turing_machine.GetConfigurationForMarkdown()
        == kExpectedConfiguration);
  }
  
  SECTION("Test Configuration Where State Name Is Just 'q'", 
      "[markdown configuration]") {
    const State kStateWithoutName = State(17, "q", 
        glm::vec2(1, 1), 5, kHaltingStateNames);
    const Direction kDirectionFive = Direction('0', '1', 'l',
        kStartingState, kStateWithoutName);
    const std::vector<State> kNewStates = {kStartingState, kStateWithoutName};
    const std::vector<Direction> kNewDirections = {kDirectionFive};
    const std::vector<char> kNewTape = {'0'};
    TuringMachine new_turing_machine = TuringMachine(kNewStates, kNewDirections, 
        kNewTape, kBlankChar, kHaltingStateNames);
    
    new_turing_machine.Update();
    const std::string kExpectedConfiguration = ";q<sub></sub>*1";
    REQUIRE(new_turing_machine.GetConfigurationForMarkdown() 
        == kExpectedConfiguration);
  }
}
