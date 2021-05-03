#include <catch2/catch.hpp>

#include "turing_machine_simulator_helper.h"

using namespace turingmachinesimulator;

/**
 * Partitions Testing As Follows:
 * Correctly Checks If Given Point Is In Given Rectangle
 * Correctly Calculates Center Of Given Rectangle
 * Correctly Calculates Location Of Direction Text
 * Correctly Evaluates Index Of Square On Tape That Was Clicked
 * Correctly Creates Directional Arrow Given 2 Points Of a Line
 * Correctly Resets The Tape
 * Correctly Adds Directions
 * Correctly Deletes States
 * Correctly Updates State Positions
 * Correctly Updates Tape Characters
 * Correctly Updates State Name
 * Correctly Updates Add Arrow Input Boxes
 */

static bool StateVariablesAreSame(const State &state, const State &state_two) {
  if (state.GetId() == state_two.GetId()
      && (state.GetStateName() == state_two.GetStateName())
      && (state.GetStateLocation() == state_two.GetStateLocation())
      && (state.GetRadius() == state_two.GetRadius())) {
    return true;
  }
  return false;
}

TEST_CASE("Test Checks If Given Point Is In Given Rectangle") {
  SECTION("Test Point Is Not In Rectangle", "[point is in rectangle]") {
    const glm::vec2 kPoint = glm::vec2(15, 15);
    const ci::Rectf kRectangle = ci::Rectf(glm::vec2(1, 1), 
        glm::vec2(14, 14));
    REQUIRE(TuringMachineSimulatorHelper::IsPointInRectangle(kPoint, kRectangle)
        == false);
  }

  SECTION("Test Point Is On Edge Of Rectangle", "[point is in rectangle]") {
    const glm::vec2 kPoint = glm::vec2(14, 14);
    const ci::Rectf kRectangle = ci::Rectf(glm::vec2(1, 1),
        glm::vec2(14, 14));
    REQUIRE(TuringMachineSimulatorHelper::IsPointInRectangle(kPoint, kRectangle) 
        == true);
  }

  SECTION("Test Point Is Inside Rectangle", "[point is in rectangle]") {
    const glm::vec2 kPoint = glm::vec2(10, 10);
    const ci::Rectf kRectangle = ci::Rectf(glm::vec2(1, 1),
        glm::vec2(14, 14));
    REQUIRE(TuringMachineSimulatorHelper::IsPointInRectangle(kPoint, kRectangle)
        == true);
  }
}

TEST_CASE("Test Correctly Calculates Center Of A Rectangle") {
  SECTION("Test Given A Square", "[rectangle center]") {
    const glm::vec2 kUpperCorner = glm::vec2(0, 0);
    const glm::vec2 kLowerCorner = glm::vec2(100, 100);
    const ci::Rectf kRectangle = ci::Rectf(kUpperCorner, kLowerCorner);
    REQUIRE(TuringMachineSimulatorHelper::GetCenterOfRectangle(kRectangle) 
        == glm::vec2(50, 50));
  }
  
  SECTION("Test Length Greater Than Height", "[rectangle center]") {
    const glm::vec2 kUpperCorner = glm::vec2(0, 0);
    const glm::vec2 kLowerCorner = glm::vec2(100, 50);
    const ci::Rectf kRectangle = ci::Rectf(kUpperCorner, kLowerCorner);
    REQUIRE(TuringMachineSimulatorHelper::GetCenterOfRectangle(kRectangle) 
        == glm::vec2(50, 25));
  }
  
  SECTION("Test Height Greater Than Length", "[rectangle center]") {
    const glm::vec2 kUpperCorner = glm::vec2(0, 0);
    const glm::vec2 kLowerCorner = glm::vec2(25, 100);
    const ci::Rectf kRectangle = ci::Rectf(kUpperCorner, kLowerCorner);
    REQUIRE(TuringMachineSimulatorHelper::GetCenterOfRectangle(kRectangle)
        == glm::vec2(12.5, 50));
  }
}

TEST_CASE("Test Correctly Calculates Location of Direction Text") {
  SECTION("Test Where 2 Points Are The Same", "[direction text]") {
    const glm::vec2 kPoint = glm::vec2(30, 30);
    REQUIRE(TuringMachineSimulatorHelper::GetDirectionTextLocation(kPoint, 
        kPoint, 10) == glm::vec2(30, 10));
  }
  
  SECTION("Test Where Point Is With Radius Of Other Point", "[direction text]") {
    const glm::vec2 kPointA = glm::vec2(30, 30);
    const glm::vec2 kPointB = glm::vec2(40, 40);
    REQUIRE(TuringMachineSimulatorHelper::GetDirectionTextLocation(kPointA, 
        kPointB, 10) == glm::vec2(30, 10));
  }
  
  SECTION("Test Where 2 Points Form Horizontal Line", "[direction text]") {
    const glm::vec2 kPointA = glm::vec2(30, 30);
    const glm::vec2 kPointB = glm::vec2(60, 30);
    REQUIRE(TuringMachineSimulatorHelper::GetDirectionTextLocation(kPointA, 
        kPointB, 10) == glm::vec2(45, 20));
  }
  
  SECTION("Test Where 2 Points Form Vertical Line", "[direction text]") {
    const glm::vec2 kPointA = glm::vec2(30, 30);
    const glm::vec2 kPointB = glm::vec2(30, 60);
    REQUIRE(TuringMachineSimulatorHelper::GetDirectionTextLocation(kPointA, 
        kPointB, 10) == glm::vec2(30, 35));
  }
  
  SECTION("Test Where 2 Points Form Diagonal With Negative Slope", 
      "[direction text]") {
    const glm::vec2 kPointA = glm::vec2(30, 30);
    const glm::vec2 kPointB = glm::vec2(10, 10);
    REQUIRE(TuringMachineSimulatorHelper::GetDirectionTextLocation(kPointA, 
        kPointB, 10) == glm::vec2(20, 10));
  }
  
  SECTION("Test Where 2 Points Form Diagonal With Positive Slope", 
      "[direction text]") {
    const glm::vec2 kPointA = glm::vec2(30, 30);
    const glm::vec2 kPointB = glm::vec2(60, 60);
    REQUIRE(TuringMachineSimulatorHelper::GetDirectionTextLocation(kPointA, 
        kPointB, 10) == glm::vec2(45, 35));
  }
}

TEST_CASE("Test Evaluation Of Index Of Square On Tape That Was Clicked") {
  SECTION("Test Where No Square On Tape Was Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(9, 9);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked
        (kClickedPoint, 8, kUpperCornerOfTape, kLowerCornerOfTape) == 8);
  }

  SECTION("Test Where Left Vertical Border Of Tape Is Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(10, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked
        (kClickedPoint, 8, kUpperCornerOfTape, kLowerCornerOfTape) == 0);
  }

  SECTION("Test Where Right Vertical Border Of Tape Is Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(26, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked
        (kClickedPoint, 8, kUpperCornerOfTape, kLowerCornerOfTape) == 7);
  }
  
  SECTION("Test Where Border Between 2 Squares Is Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(18, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked
        (kClickedPoint, 8, kUpperCornerOfTape, kLowerCornerOfTape) == 3);
  }
  
  SECTION("Test Where 1st Square On Tape Was Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(11, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked
        (kClickedPoint, 8, kUpperCornerOfTape, kLowerCornerOfTape) == 0);
  }
  
  SECTION("Test Where Last Square On Tape Was Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(25, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked
        (kClickedPoint, 8, kUpperCornerOfTape, kLowerCornerOfTape) == 7);
  }
  
  SECTION("Test Where Square In Middle Of Tape Was Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(21, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(TuringMachineSimulatorHelper::GetIndexOfSquareOfTapeClicked
        (kClickedPoint, 8,  kUpperCornerOfTape, kLowerCornerOfTape) == 5);
  }
}

TEST_CASE("Test Correctly Calculates Directional Arrow") {
  SECTION("Test Starting And Ending Point Are The Same", "[arrow]") {
    const glm::vec2 kStartingPoint = glm::vec2(10, 10);
    const glm::vec2 kEndingPoint = glm::vec2(10, 10);
    const std::tuple<glm::vec2, glm::vec2, glm::vec2>  kArrowPoints =
        TuringMachineSimulatorHelper::GetArrow(kStartingPoint, kEndingPoint);
    
    REQUIRE(std::get<0>(kArrowPoints) == glm::vec2(5.919179382, 19.12945251));
    REQUIRE(std::get<1>(kArrowPoints) == glm::vec2(10, 10));
    REQUIRE(std::get<2>(kArrowPoints) == glm::vec2(5.919179382, .8705474927));
  }

  SECTION("Test Starting And Ending Point Have Same X Value", "[arrow]") {
    const glm::vec2 kStartingPoint = glm::vec2(10, 30);
    const glm::vec2 kEndingPoint = glm::vec2(10, 10);
    const std::tuple<glm::vec2, glm::vec2, glm::vec2>  kArrowPoints =
        TuringMachineSimulatorHelper::GetArrow(kStartingPoint, kEndingPoint);

    REQUIRE(std::get<0>(kArrowPoints) == glm::vec2(19.12945251, 19.08082062));
    REQUIRE(std::get<1>(kArrowPoints) == glm::vec2(10, 15));
    const glm::vec2 kPointThree = std::get<2>(kArrowPoints);
    REQUIRE(kPointThree.x == Approx(.8705474919));
    REQUIRE(kPointThree.y == Approx(19.08082062));
  }

  SECTION("Test Starting And Ending Point Have Same Y Value", "[arrow]") {
    const glm::vec2 kStartingPoint = glm::vec2(2, 10);
    const glm::vec2 kEndingPoint = glm::vec2(10, 10);
    const std::tuple<glm::vec2, glm::vec2, glm::vec2>  kArrowPoints =
        TuringMachineSimulatorHelper::GetArrow(kStartingPoint, kEndingPoint);

    REQUIRE(std::get<0>(kArrowPoints) == glm::vec2(3.919179382, 19.12945251));
    REQUIRE(std::get<1>(kArrowPoints) == glm::vec2(8, 10));
    REQUIRE(std::get<2>(kArrowPoints) == glm::vec2(3.919179382, .8705474927));
  }

  SECTION("Test Starting And Ending Point Have Negative Slope", "[arrow]") {
    const glm::vec2 kStartingPoint = glm::vec2(2, 20);
    const glm::vec2 kEndingPoint = glm::vec2(10, 10);
    const std::tuple<glm::vec2, glm::vec2, glm::vec2>  kArrowPoints =
        TuringMachineSimulatorHelper::GetArrow(kStartingPoint, kEndingPoint);

    REQUIRE(std::get<0>(kArrowPoints) == glm::vec2(12.57963628, 21.38970931));
    REQUIRE(std::get<1>(kArrowPoints) == glm::vec2(8, 12.5));
    const glm::vec2 kPointThree = std::get<2>(kArrowPoints);
    REQUIRE(kPointThree.x == Approx(-1.67817314));
    REQUIRE(kPointThree.y == Approx(9.98346177));
  }

  SECTION("Test Starting And Ending Point Have Positive Slope", "[arrow]") {
    const glm::vec2 kStartingPoint = glm::vec2(10, 10);
    const glm::vec2 kEndingPoint = glm::vec2(20, 40);
    const std::tuple<glm::vec2, glm::vec2, glm::vec2>  kArrowPoints =
        TuringMachineSimulatorHelper::GetArrow(kStartingPoint, kEndingPoint);

    const glm::vec2 kPointOne = std::get<0>(kArrowPoints);
    REQUIRE(kPointOne.x == Approx(7.548572098));
    REQUIRE(kPointOne.y == Approx(31.51558001));
    REQUIRE(std::get<1>(kArrowPoints) == glm::vec2(17.5, 32.5));
    const glm::vec2 kPointThree = std::get<2>(kArrowPoints);
    REQUIRE(kPointThree.x == Approx(24.87049033));
    REQUIRE(kPointThree.y == Approx(25.74160727));
  }
}

TEST_CASE("Test Resets Tape Correctly") {
  const char kBlankCharacter = '-';
  const std::vector<char> kExpectedTape = {kBlankCharacter, kBlankCharacter,
      kBlankCharacter, kBlankCharacter, kBlankCharacter, kBlankCharacter, 
      kBlankCharacter, kBlankCharacter};
  
  SECTION("Test Given An Empty Vector", "[reset tape]") {
    std::vector<char> tape_to_reset = {};
    TuringMachineSimulatorHelper::ResetTape(tape_to_reset);
    REQUIRE(tape_to_reset == kExpectedTape);
  }
  
  SECTION("Test Given A Vector With Less Than 8 Characters", "[reset tape]") {
    std::vector<char> tape_to_reset = {'a', 'e', 'i', 'o', 'u', 'y', '-'};
    TuringMachineSimulatorHelper::ResetTape(tape_to_reset);
    REQUIRE(tape_to_reset == kExpectedTape);
  }
  
  SECTION("Test Given A Vector With More THan 8 Characters", "[reset tape]") {
    std::vector<char> tape_to_reset = {'1', '2', '3', '4', '5', '6', '7', '8', 
        '9'};
    TuringMachineSimulatorHelper::ResetTape(tape_to_reset);
    REQUIRE(tape_to_reset == kExpectedTape);
  }
  
  SECTION("Test Given A Vector of 8 Characters", "[reset tape]") {
    std::vector<char> tape_to_reset = {'!', '@', '#', '$', '%', '^', '&', '*'};
    TuringMachineSimulatorHelper::ResetTape(tape_to_reset);
    REQUIRE(tape_to_reset == kExpectedTape);
  }
}

TEST_CASE("Test Adding Directions") {
  const State kStartingState = State(1, "q1", 
      glm::vec2(1, 1), 5);
  const State kHaltingState = State(5, "qh", 
      glm::vec2(5, 6), 3);
  
  SECTION("Test Invalid Direction Inputs", "[add direction]") {
    std::vector<Direction> directions = {};
    const std::vector<State> kStates = {};
    const std::vector<std::string> kInputs = {"a", "b", "l", "q1", "qh"};
    TuringMachineSimulatorHelper::AddDirection(kInputs, directions, kStates);
    REQUIRE(directions.empty());
  }
  
  SECTION("Test Valid Inputs", "[add direction]") {
    std::vector<Direction> directions = {};
    const std::vector<State> kStates = {kStartingState, kHaltingState};
    const std::vector<std::string> kInputs = {"a", "b", "l", "q1", "qh"};
    TuringMachineSimulatorHelper::AddDirection(kInputs, directions, kStates);
    REQUIRE(directions.size() == 1);
    const Direction kExpectedDirection = Direction('a', 'b', 'l', 
        kStartingState, kHaltingState);
    REQUIRE(directions.at(0).Equals(kExpectedDirection));
  }
}

TEST_CASE("Test State Deletion") {
  const State kStartingState = State(1, "q1",
      glm::vec2(1, 1), 5);
  const State kHaltingState = State(5, "qh",
      glm::vec2(5, 6), 3);
  const State kNthState = State(2, "qn", 
      glm::vec2(9, 8), 7);
  
  SECTION("Test No Directions Attached To Deleted State", "[state deletion]") {
    std::vector<State> states = {kStartingState, kHaltingState};
    std::vector<Direction> directions = {};
    TuringMachineSimulatorHelper::DeleteGivenState(kStartingState, states, 
        directions);
    REQUIRE(directions.empty());
    REQUIRE(states.size() == 1);
    const State kStateOutput = states.at(0);
    REQUIRE(kStateOutput.GetId() == 5);
    REQUIRE(kStateOutput.GetStateName() == "qh");
    REQUIRE(kStateOutput.GetStateLocation() == glm::vec2(5, 6));
    REQUIRE(kStateOutput.GetRadius() == 3);
  }
  
  SECTION("Test Has Direction Going From Deleted State", "[state deletion]") {
    const Direction kDirection = Direction('0', '1', 'n',
        kStartingState, kHaltingState);
    std::vector<State> states = {kStartingState, kHaltingState};
    std::vector<Direction> directions = {kDirection};
    TuringMachineSimulatorHelper::DeleteGivenState(kStartingState, states,
        directions);
    REQUIRE(directions.empty());
    REQUIRE(states.size() == 1);
    const State kStateOutput = states.at(0);
    REQUIRE(kStateOutput.GetId() == 5);
    REQUIRE(kStateOutput.GetStateName() == "qh");
    REQUIRE(kStateOutput.GetStateLocation() == glm::vec2(5, 6));
    REQUIRE(kStateOutput.GetRadius() == 3);
  }
  
  SECTION("Test Has Direction Going To Deleted State", "[state deletion]") {
    const Direction kDirection = Direction('1', '0', 'r',
        kNthState, kStartingState);
    std::vector<State> states = {kStartingState, kHaltingState, kNthState};
    std::vector<Direction> directions = {kDirection};
    TuringMachineSimulatorHelper::DeleteGivenState(kStartingState, states,
        directions);
    REQUIRE(directions.empty());
    REQUIRE(states.size() == 2);
    const std::vector<State> kExpectedStates = {kHaltingState, kNthState};
    for (size_t i = 0; i < kExpectedStates.size(); i++) {
      const State kStateOutput = states.at(i);
      const State kExpectedState = kExpectedStates.at(i);
      REQUIRE(StateVariablesAreSame(kStateOutput, kExpectedState));
    }
  }
  
  SECTION("Test Has Self-Loop To Deleted State", "[state deletion]") {
    const Direction kDirection = Direction('1', '0', 'r',
        kStartingState, kStartingState);
    std::vector<State> states = {kStartingState, kHaltingState, kNthState};
    std::vector<Direction> directions = {kDirection};
    TuringMachineSimulatorHelper::DeleteGivenState(kStartingState, states,
        directions);
    REQUIRE(directions.empty());
    REQUIRE(states.size() == 2);
    const std::vector<State> kExpectedStates = {kHaltingState, kNthState};
    for (size_t i = 0; i < kExpectedStates.size(); i++) {
      const State kStateOutput = states.at(i);
      const State kExpectedState = kExpectedStates.at(i);
      REQUIRE(StateVariablesAreSame(kStateOutput, kExpectedState));
    }
  }

  SECTION("Test Has Multiple Directions To Deleted State", "[state deletion]") {
    const Direction kDirection = Direction('1', '0', 'r',
        kStartingState, kStartingState);
    const Direction kDirectionTwo = Direction('0', '1', 'l', 
        kNthState, kHaltingState);
    const Direction kDirectionThree = Direction('9', '8', 'r', 
        kStartingState, kHaltingState);
    std::vector<State> states = {kStartingState, kHaltingState, kNthState};
    std::vector<Direction> directions = {kDirection, kDirectionTwo, 
        kDirectionThree};
    TuringMachineSimulatorHelper::DeleteGivenState(kStartingState, states,
        directions);
    REQUIRE(directions.size() == 1);
    REQUIRE(directions.at(0).Equals(kDirectionTwo));
    REQUIRE(states.size() == 2);
    const std::vector<State> kExpectedStates = {kHaltingState, kNthState};
    for (size_t i = 0; i < kExpectedStates.size(); i++) {
      const State kStateOutput = states.at(i);
      const State kExpectedState = kExpectedStates.at(i);
      REQUIRE(StateVariablesAreSame(kStateOutput, kExpectedState));
    }
  }
}

TEST_CASE("Test Updates State Positions") {
  const State kStartingState = State(1, "q1",
      glm::vec2(1, 1), 5);
  const State kHaltingState = State(5, "qh",
      glm::vec2(5, 6), 3);
  
  SECTION("Test Click Same As Current Position", "[state position]") {
    State clicked_state = State(2, "qn",
        glm::vec2(9, 8), 7);
    std::vector<State> states = {kStartingState, kHaltingState, clicked_state};
    const glm::vec2 kUpdatedPosition = glm::vec2(9, 8);
    TuringMachineSimulatorHelper::UpdateStatePosition(clicked_state, states, 
        kUpdatedPosition);
    const State kExpectedClickedState = State(2, "qn",
        glm::vec2(9, 8), 7);
    REQUIRE(clicked_state.Equals(kExpectedClickedState));
    REQUIRE(states.size() == 3);
    const std::vector<State> kExpectedStates = {kStartingState, kHaltingState, 
        kExpectedClickedState};
    for (size_t i = 0; i < kExpectedStates.size(); i++) {
      const State kStateOutput = states.at(i);
      const State kExpectedState = kExpectedStates.at(i);
      REQUIRE(StateVariablesAreSame(kStateOutput, kExpectedState));
    }
  }
  
  SECTION("Test Click Is A New Position", "[state position]") {
    State clicked_state = State(2, "qn",
        glm::vec2(9, 8), 7);
    std::vector<State> states = {kStartingState, clicked_state, kHaltingState};
    const glm::vec2 kUpdatedPosition = glm::vec2(0, 0);
    TuringMachineSimulatorHelper::UpdateStatePosition(clicked_state, states,
        kUpdatedPosition);
    const State kExpectedClickedState = State(2, "qn",
        glm::vec2(0, 0), 7);
    REQUIRE(clicked_state.Equals(kExpectedClickedState));
    REQUIRE(states.size() == 3);
    const std::vector<State> kExpectedStates = {kStartingState, 
        kExpectedClickedState, kHaltingState};
    for (size_t i = 0; i < kExpectedStates.size(); i++) {
      const State kStateOutput = states.at(i);
      const State kExpectedState = kExpectedStates.at(i);
      REQUIRE(StateVariablesAreSame(kStateOutput, kExpectedState));
    }
  }
}

TEST_CASE("Test Tape Characters Are Updated Correctly") {
  SECTION("Test Return Key Is Pressed", "[update tape]") {
    std::vector<char> tape = {'a', 'b', 'c'};
    int character_to_edit = 0;
    const int kReturnCode = 13;
    character_to_edit = TuringMachineSimulatorHelper::UpdateTapeCharacter(tape,
        character_to_edit, ' ', kReturnCode);
    REQUIRE(character_to_edit == 3);
    const std::vector<char> kExpectedTape = {'a', 'b', 'c'};
    REQUIRE(tape == kExpectedTape);
  }
  
  SECTION("Test Backspace Is Pressed", "[update tape]") {
    std::vector<char> tape = {'a', 'b', 'c'};
    int character_to_edit = 0;
    const int kBackspaceCode = 8;
    character_to_edit = TuringMachineSimulatorHelper::UpdateTapeCharacter(tape,
        character_to_edit, ' ', kBackspaceCode);
    REQUIRE(character_to_edit == 0);
    const std::vector<char> kExpectedTape = {'-', 'b', 'c'};
    REQUIRE(tape == kExpectedTape);
  }
  
  SECTION("Test Number Is Typed", "[update tape]") {
    std::vector<char> tape = {'a', 'b', 'c'};
    int character_to_edit = 2;
    character_to_edit = TuringMachineSimulatorHelper::UpdateTapeCharacter(tape,
        character_to_edit, '2', -1);
    REQUIRE(character_to_edit == 2);
    const std::vector<char> kExpectedTape = {'a', 'b', '2'};
    REQUIRE(tape == kExpectedTape);
  }
  
  SECTION("Test Letter Is Typed", "[update tape]") {
    std::vector<char> tape = {'a', 'b', 'c'};
    int character_to_edit = 2;
    character_to_edit = TuringMachineSimulatorHelper::UpdateTapeCharacter(tape,
        character_to_edit, 'a', -1);
    REQUIRE(character_to_edit == 2);
    const std::vector<char> kExpectedTape = {'a', 'b', 'a'};
    REQUIRE(tape == kExpectedTape);
  }
  
  SECTION("Test Special Character Is Typed", "[update tape]") {
    std::vector<char> tape = {'a', 'b', 'c'};
    int character_to_edit = 1;
    character_to_edit = TuringMachineSimulatorHelper::UpdateTapeCharacter(tape,
        character_to_edit, '~', -1);
    REQUIRE(character_to_edit == 1);
    const std::vector<char> kExpectedTape = {'a', '~', 'c'};
    REQUIRE(tape == kExpectedTape);
  }
}

TEST_CASE("Test Updating State Names") {
  const State kStartingState = State(1, "q1",
      glm::vec2(1, 1), 5);
  const State kHaltingState = State(5, "qh",
      glm::vec2(5, 6), 3);
  
  SECTION("Test Return Key Is Pressed", "[state name]") {
    State state_to_update = State(7, "qn", 
        glm::vec2(1, 5), 7);
    std::vector<State> states = {kStartingState, state_to_update, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'L', 
        kStartingState, state_to_update);
    std::vector<Direction> directions = {kDirectionOne};
    const int kReturnCode = 13;
    const bool kIsBeingEdited = TuringMachineSimulatorHelper::UpdateStateName
        (states, directions, state_to_update, ' ', 
         kReturnCode);
    // check that the method correctly identifies the return code (no more 
    // editing)
    REQUIRE(kIsBeingEdited == false);
    // check that the state names are as expected (unedited)
    const State kUpdatedState = State(7, "qn",
        glm::vec2(1, 5), 7);
    REQUIRE(state_to_update.IsEmpty());
    REQUIRE(states.size() == 3);
    const std::vector<State> kExpectedStates = {kStartingState, kUpdatedState,
        kHaltingState};
    for (size_t i = 0; i < kExpectedStates.size(); i++) {
      const State kStateOutput = states.at(i);
      const State kExpectedState = kExpectedStates.at(i);
      REQUIRE(StateVariablesAreSame(kStateOutput, kExpectedState));
    }
    // check that the directions are as expected (unedited)
    REQUIRE(directions.size() == 1);
    const Direction kUpdatedDirection = directions.at(0);
    REQUIRE(kUpdatedDirection.Equals(kDirectionOne));
    REQUIRE(StateVariablesAreSame(kUpdatedDirection.GetStateToMoveFrom(), 
        kStartingState));
    REQUIRE(StateVariablesAreSame(kUpdatedDirection.GetStateToMoveTo(), 
        kUpdatedState));
  }
  
  SECTION("Test Backspace Is Pressed", "[state name]") {
    State state_to_update = State(7, "qexample",
        glm::vec2(1, 5), 7);
    std::vector<State> states = {kStartingState, state_to_update, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'L',
        state_to_update, state_to_update);
    std::vector<Direction> directions = {kDirectionOne};
    const int kBackspaceCode = 8;
    const bool kIsBeingEdited = TuringMachineSimulatorHelper::UpdateStateName
        (states, directions, state_to_update, ' ', 
         kBackspaceCode);

    REQUIRE(kIsBeingEdited);
    // check that correct state name is updated
    const State kUpdatedState = State(7, "qexampl",
        glm::vec2(1, 5), 7);
    REQUIRE(state_to_update.Equals(kUpdatedState));
    REQUIRE(states.size() == 3);
    const std::vector<State> kExpectedStates = {kStartingState, kUpdatedState,
        kHaltingState};
    for (size_t i = 0; i < kExpectedStates.size(); i++) {
      const State kStateOutput = states.at(i);
      const State kExpectedState = kExpectedStates.at(i);
      REQUIRE(StateVariablesAreSame(kStateOutput, kExpectedState));
    }
    // check that directions involving the edited state are updated correctly
    REQUIRE(directions.size() == 1);
    const Direction kExpectedDirection = Direction('0', '1', 
        'L', kUpdatedState, kUpdatedState);
    const Direction kUpdatedDirection = directions.at(0);
    REQUIRE(kUpdatedDirection.Equals(kExpectedDirection));
    REQUIRE(StateVariablesAreSame(kUpdatedDirection.GetStateToMoveFrom(),
        kUpdatedState));
    REQUIRE(StateVariablesAreSame(kUpdatedDirection.GetStateToMoveTo(),
        kUpdatedState));
  }
  
  SECTION("Test Backspace With State Named 'q'", "[state name]") {
    State state_to_update = State(7, "q",
        glm::vec2(1, 5), 7);
    std::vector<State> states = {kStartingState, state_to_update, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'L',
        kStartingState, state_to_update);
    std::vector<Direction> directions = {kDirectionOne};
    const int kBackspaceCode = 8;
    const bool kIsBeingEdited = TuringMachineSimulatorHelper::UpdateStateName
        (states, directions, state_to_update, ' ', 
         kBackspaceCode);
    
    REQUIRE(kIsBeingEdited);
    // check that the state name does not change
    const State kUpdatedState = State(7, "q",
        glm::vec2(1, 5), 7);
    REQUIRE(state_to_update.Equals(kUpdatedState));
    REQUIRE(states.size() == 3);
    const std::vector<State> kExpectedStates = {kStartingState, kUpdatedState,
        kHaltingState};
    for (size_t i = 0; i < kExpectedStates.size(); i++) {
      const State kStateOutput = states.at(i);
      const State kExpectedState = kExpectedStates.at(i);
      REQUIRE(StateVariablesAreSame(kStateOutput, kExpectedState));
    }
    // check that the directions are as expected (unedited)
    REQUIRE(directions.size() == 1);
    const Direction kExpectedDirection = Direction('0', '1', 
        'L', kStartingState, kUpdatedState);
    const Direction kUpdatedDirection = directions.at(0);
    REQUIRE(kUpdatedDirection.Equals(kExpectedDirection));
    REQUIRE(StateVariablesAreSame(kUpdatedDirection.GetStateToMoveFrom(),
        kStartingState));
    REQUIRE(StateVariablesAreSame(kUpdatedDirection.GetStateToMoveTo(),
        kUpdatedState));
  }
  
  SECTION("Test Number Is Typed", "[state name]") {
    State state_to_update = State(7, "q2",
        glm::vec2(1, 5), 7);
    std::vector<State> states = {kStartingState, state_to_update, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'L',
        state_to_update, kStartingState);
    std::vector<Direction> directions = {kDirectionOne};
    const bool kIsBeingEdited = TuringMachineSimulatorHelper::UpdateStateName
        (states, directions, state_to_update, '3', 
         -1);
    
    REQUIRE(kIsBeingEdited);
    // check that the correct state name is updated
    const State kUpdatedState = State(7, "q23",
        glm::vec2(1, 5), 7);
    REQUIRE(state_to_update.Equals(kUpdatedState));
    REQUIRE(states.size() == 3);
    const std::vector<State> kExpectedStates = {kStartingState, kUpdatedState,
        kHaltingState};
    for (size_t i = 0; i < kExpectedStates.size(); i++) {
      const State kStateOutput = states.at(i);
      const State kExpectedState = kExpectedStates.at(i);
      REQUIRE(StateVariablesAreSame(kStateOutput, kExpectedState));
    }
    // check that directions involving the edited state are updated correctly
    REQUIRE(directions.size() == 1);
    const Direction kExpectedDirection = Direction('0', '1',
        'L', kUpdatedState, kStartingState);
    const Direction kUpdatedDirection = directions.at(0);
    REQUIRE(kUpdatedDirection.Equals(kExpectedDirection));
    REQUIRE(StateVariablesAreSame(kUpdatedDirection.GetStateToMoveFrom(),
        kUpdatedState));
    REQUIRE(StateVariablesAreSame(kUpdatedDirection.GetStateToMoveTo(),
        kStartingState));
  }
  
  SECTION("Test Letter Is Typed", "[state name]") {
    State state_to_update = State(7, "q1",
        glm::vec2(1, 5), 7);
    std::vector<State> states = {kStartingState, state_to_update, kHaltingState};
    const Direction kDirectionOne = Direction('0', '1', 'L',
        state_to_update, kStartingState);
    const Direction kDirectionTwo = Direction('1', '0', 'r', state_to_update, 
        kHaltingState);
    std::vector<Direction> directions = {kDirectionOne, kDirectionTwo};
    const bool kIsBeingEdited = TuringMachineSimulatorHelper::UpdateStateName
        (states, directions, state_to_update, 'a', -1);
    
    REQUIRE(kIsBeingEdited);
    // check that the correct state name is updated
    const State kUpdatedState = State(7, "q1a",
        glm::vec2(1, 5), 7);
    REQUIRE(state_to_update.Equals(kUpdatedState));
    REQUIRE(states.size() == 3);
    const std::vector<State> kExpectedStates = {kStartingState, kUpdatedState,
        kHaltingState};
    for (size_t i = 0; i < kExpectedStates.size(); i++) {
      const State kStateOutput = states.at(i);
      const State kExpectedState = kExpectedStates.at(i);
      REQUIRE(StateVariablesAreSame(kStateOutput, kExpectedState));
    }
    // check that directions involving the edited state are updated correctly
    REQUIRE(directions.size() == 2);
    const Direction kExpectedDirectionOne = Direction('0', '1',
        'L', kUpdatedState, kStartingState);
    const Direction kExpectedDirectionTwo = Direction('1', '0', 
        'r', state_to_update, kHaltingState);
    const std::vector<Direction> kExpectedDirections = {kExpectedDirectionOne,
        kExpectedDirectionTwo};
    for (size_t i = 0; i < kExpectedDirections.size(); i++) {
      const Direction kDirectionOutput = directions.at(i);
      const Direction kExpectedDirection = directions.at(i);
      REQUIRE(kDirectionOutput.Equals(kExpectedDirection));
      REQUIRE(StateVariablesAreSame(kDirectionOutput.GetStateToMoveFrom(),
          kExpectedDirection.GetStateToMoveFrom()));
      REQUIRE(StateVariablesAreSame(kDirectionOutput.GetStateToMoveTo(),
          kExpectedDirection.GetStateToMoveTo()));
    }
  }
  
  SECTION("Test Special Character Is Typed", "[state name]") {
    State state_to_update = State(7, "qh",
        glm::vec2(1, 5), 7);
    std::vector<State> states = {kStartingState, state_to_update, kHaltingState};
    std::vector<Direction> directions = {};
    const bool kIsBeingEdited = TuringMachineSimulatorHelper::UpdateStateName
        (states, directions, state_to_update, '?', 
         -1);
    
    REQUIRE(kIsBeingEdited);
    // check that the correct state name is updated
    const State kUpdatedState = State(7, "qh?",
        glm::vec2(1, 5), 7);
    REQUIRE(state_to_update.Equals(kUpdatedState));
    REQUIRE(states.size() == 3);
    const std::vector<State> kExpectedStates = {kStartingState, kUpdatedState,
        kHaltingState};
    for (size_t i = 0; i < kExpectedStates.size(); i++) {
      const State kStateOutput = states.at(i);
      const State kExpectedState = kExpectedStates.at(i);
      REQUIRE(StateVariablesAreSame(kStateOutput, kExpectedState));
    }
    // check that directions is still empty
    REQUIRE(directions.empty());
  }
}

TEST_CASE("Test Updates Add Arrow Input Box") {
  SECTION("Test Return Key Is Pressed", "[update add arrow input]") {
    std::vector<std::string> add_arrow_inputs = {"a", "b", "r", "q1", "qn"};
    const int kReturnCode = 13;
    const int kIndexBeingEdited = TuringMachineSimulatorHelper
        ::UpdateAddArrowInputs(add_arrow_inputs, 2,
        ' ', kReturnCode);
    REQUIRE(kIndexBeingEdited == 5);
    const std::vector<std::string> kExpectedInputs = {"a", "b", "r", "q1", "qn"};
    REQUIRE(add_arrow_inputs == kExpectedInputs);
  }
  
  SECTION("Test Backspace Is Pressed", "[update add arrow input]") {
    std::vector<std::string> add_arrow_inputs = {"a", "b", "r", "q1", "qn"};
    const int kBackspaceCode = 8;
    const int kIndexBeingEdited = TuringMachineSimulatorHelper
        ::UpdateAddArrowInputs(add_arrow_inputs, 2, 
        ' ', kBackspaceCode);
    REQUIRE(kIndexBeingEdited == 2);
    const std::vector<std::string> kExpectedInputs = {"a", "b", "", "q1", "qn"};
    REQUIRE(add_arrow_inputs == kExpectedInputs);
  }
  
  SECTION("Test Backspace Is Pressed And Input Is Empty", 
      "[update add arrow input]") {
    std::vector<std::string> add_arrow_inputs = {"a", "b", "r", "", "qn"};
    const int kBackspaceCode = 8;
    const int kIndexBeingEdited = TuringMachineSimulatorHelper
        ::UpdateAddArrowInputs(add_arrow_inputs, 3, 
        ' ', kBackspaceCode);
    REQUIRE(kIndexBeingEdited == 3);
    const std::vector<std::string> kExpectedInputs = {"a", "b", "r", "", "qn"};
    REQUIRE(add_arrow_inputs == kExpectedInputs);
  }
  
  SECTION("Test Number Is Typed", "[update add arrow input]") {
    std::vector<std::string> add_arrow_inputs = {"", "b", "r", "q1", "qn"};
    const int kIndexBeingEdited = TuringMachineSimulatorHelper
        ::UpdateAddArrowInputs(add_arrow_inputs, 0,
        '1', -1);
    REQUIRE(kIndexBeingEdited == 0);
    const std::vector<std::string> kExpectedInputs = {"1", "b", "r", "q1", "qn"};
    REQUIRE(add_arrow_inputs == kExpectedInputs);
  }
  
  SECTION("Test Letter Is Typed", "[update add arrow input]") {
    std::vector<std::string> add_arrow_inputs = {"a", "b", "r", "qn", "q1"};
    const int kIndexBeingEdited = TuringMachineSimulatorHelper
        ::UpdateAddArrowInputs(add_arrow_inputs, 4,
        'a', -1);
    REQUIRE(kIndexBeingEdited == 4);
    const std::vector<std::string> kExpectedInputs = {"a", "b", "r", "qn", "q1a"};
    REQUIRE(add_arrow_inputs == kExpectedInputs);
  }
  
  SECTION("Test Special Character Is Typed", "[update add arrow input]") {
    std::vector<std::string> add_arrow_inputs = {"a", "b", "r", "qn", "q1"};
    const int kIndexBeingEdited = TuringMachineSimulatorHelper
        ::UpdateAddArrowInputs(add_arrow_inputs, 1,
        '?', -1);
    REQUIRE(kIndexBeingEdited == 1);
    const std::vector<std::string> kExpectedInputs = {"a", "b?", "r", "qn", "q1"};
    REQUIRE(add_arrow_inputs == kExpectedInputs);
  }
}
