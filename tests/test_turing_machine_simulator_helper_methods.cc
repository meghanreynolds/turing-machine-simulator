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
 */
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

    REQUIRE(std::get<0>(kArrowPoints) == glm::vec2(19.12945251, 24.08082062));
    REQUIRE(std::get<1>(kArrowPoints) == glm::vec2(10, 20));
    const glm::vec2 kPointThree = std::get<2>(kArrowPoints);
    REQUIRE(kPointThree.x == Approx(.8705474919));
    REQUIRE(kPointThree.y == Approx(24.08082062));
  }

  SECTION("Test Starting And Ending Point Have Same Y Value", "[arrow]") {
    const glm::vec2 kStartingPoint = glm::vec2(2, 10);
    const glm::vec2 kEndingPoint = glm::vec2(10, 10);
    const std::tuple<glm::vec2, glm::vec2, glm::vec2>  kArrowPoints =
        TuringMachineSimulatorHelper::GetArrow(kStartingPoint, kEndingPoint);

    REQUIRE(std::get<0>(kArrowPoints) == glm::vec2(1.919179382, 19.12945251));
    REQUIRE(std::get<1>(kArrowPoints) == glm::vec2(6, 10));
    REQUIRE(std::get<2>(kArrowPoints) == glm::vec2(1.919179382, .8705474927));
  }

  SECTION("Test Starting And Ending Point Have Negative Slope", "[arrow]") {
    const glm::vec2 kStartingPoint = glm::vec2(2, 20);
    const glm::vec2 kEndingPoint = glm::vec2(10, 10);
    const std::tuple<glm::vec2, glm::vec2, glm::vec2>  kArrowPoints =
        TuringMachineSimulatorHelper::GetArrow(kStartingPoint, kEndingPoint);

    REQUIRE(std::get<0>(kArrowPoints) == glm::vec2(10.57963628, 23.88970931));
    REQUIRE(std::get<1>(kArrowPoints) == glm::vec2(6, 15));
    const glm::vec2 kPointThree = std::get<2>(kArrowPoints);
    REQUIRE(kPointThree.x == Approx(-3.67817314));
    REQUIRE(kPointThree.y == Approx(12.483461777));
  }

  SECTION("Test Starting And Ending Point Have Positive Slope", "[arrow]") {
    const glm::vec2 kStartingPoint = glm::vec2(10, 10);
    const glm::vec2 kEndingPoint = glm::vec2(20, 40);
    const std::tuple<glm::vec2, glm::vec2, glm::vec2>  kArrowPoints =
        TuringMachineSimulatorHelper::GetArrow(kStartingPoint, kEndingPoint);

    const glm::vec2 kPointOne = std::get<0>(kArrowPoints);
    REQUIRE(kPointOne.x == Approx(5.048572098));
    REQUIRE(kPointOne.y == Approx(24.01558001));
    REQUIRE(std::get<1>(kArrowPoints) == glm::vec2(15, 25));
    const glm::vec2 kPointThree = std::get<2>(kArrowPoints);
    REQUIRE(kPointThree.x == Approx(22.37049033));
    REQUIRE(kPointThree.y == Approx(18.24160727));
  }
}
