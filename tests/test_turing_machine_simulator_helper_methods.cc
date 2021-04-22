#include <catch2/catch.hpp>

#include "turing_machine_simulator_helper.h"

using namespace turingmachinesimulator;

/**
 * Partitions Testing As Follows:
 * Correctly Checks If Given Point Is In Given Rectangle
 * Correctly Calculates Location Of Direction Text
 * Correctly Evaluates Index Of Square On Tape That Was Clicked
 */
TEST_CASE("Test Checks If Given Point Is In Given Rectangle") {
  const TuringMachineSimulatorHelper kHelperMethods =
      TuringMachineSimulatorHelper();
  
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

TEST_CASE("Test Correctly Calculates Location of Direction Text") {
  const TuringMachineSimulatorHelper kHelperMethods =
      TuringMachineSimulatorHelper();
  
  SECTION("Test Where 2 Points Are The Same", "[direction text]") {
    const glm::vec2 kPoint = glm::vec2(30, 30);
    REQUIRE(kHelperMethods.GetDirectionTextLocation(kPoint, kPoint, 10) 
        == glm::vec2(30, 10));
  }
  
  SECTION("Test Where Point Is With Radius Of Other Point", "[direction text]") {
    const glm::vec2 kPointA = glm::vec2(30, 30);
    const glm::vec2 kPointB = glm::vec2(40, 40);
    REQUIRE(kHelperMethods.GetDirectionTextLocation(kPointA, kPointB, 10) 
        == glm::vec2(30, 10));
  }
  
  SECTION("Test Where 2 Points Form Horizontal Line", "[direction text]") {
    const glm::vec2 kPointA = glm::vec2(30, 30);
    const glm::vec2 kPointB = glm::vec2(60, 30);
    REQUIRE(kHelperMethods.GetDirectionTextLocation(kPointA, kPointB, 10) 
        == glm::vec2(45, 20));
  }
  
  SECTION("Test Where 2 Points Form Vertical Line", "[direction text]") {
    const glm::vec2 kPointA = glm::vec2(30, 30);
    const glm::vec2 kPointB = glm::vec2(30, 60);
    REQUIRE(kHelperMethods.GetDirectionTextLocation(kPointA, kPointB, 10) 
        == glm::vec2(30, 35));
  }
  
  SECTION("Test Where 2 Points Form Diagonal With Negative Slope", 
      "[direction text]") {
    const glm::vec2 kPointA = glm::vec2(30, 30);
    const glm::vec2 kPointB = glm::vec2(10, 10);
    REQUIRE(kHelperMethods.GetDirectionTextLocation(kPointA, kPointB, 10) 
        == glm::vec2(20, 10));
  }
  
  SECTION("Test Where 2 Points Form Diagonal With Positive Slope", 
      "[direction text]") {
    const glm::vec2 kPointA = glm::vec2(30, 30);
    const glm::vec2 kPointB = glm::vec2(60, 60);
    REQUIRE(kHelperMethods.GetDirectionTextLocation(kPointA, kPointB, 10) 
        == glm::vec2(45, 35));
  }
}

TEST_CASE("Test Evaluation Of Index Of Square On Tape That Was Clicked") {
  const TuringMachineSimulatorHelper kHelperMethods =
      TuringMachineSimulatorHelper();
  
  SECTION("Test Where No Square On Tape Was Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(9, 9);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(kHelperMethods.GetIndexOfSquareOfTapeClicked(kClickedPoint, 
        kUpperCornerOfTape, kLowerCornerOfTape) == 8);
  }

  SECTION("Test Where Left Vertical Border Of Tape Is Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(10, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(kHelperMethods.GetIndexOfSquareOfTapeClicked(kClickedPoint,
        kUpperCornerOfTape, kLowerCornerOfTape) == 0);
  }

  SECTION("Test Where Right Vertical Border Of Tape Is Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(26, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(kHelperMethods.GetIndexOfSquareOfTapeClicked(kClickedPoint,
        kUpperCornerOfTape, kLowerCornerOfTape) == 7);
  }
  
  SECTION("Test Where Border Between 2 Squares Is Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(18, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(kHelperMethods.GetIndexOfSquareOfTapeClicked(kClickedPoint,
        kUpperCornerOfTape, kLowerCornerOfTape) == 3);
  }
  
  SECTION("Test Where 1st Square On Tape Was Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(11, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(kHelperMethods.GetIndexOfSquareOfTapeClicked(kClickedPoint,
        kUpperCornerOfTape, kLowerCornerOfTape) == 0);
  }
  
  SECTION("Test Where Last Square On Tape Was Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(25, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(kHelperMethods.GetIndexOfSquareOfTapeClicked(kClickedPoint,
        kUpperCornerOfTape, kLowerCornerOfTape) == 7);
  }
  
  SECTION("Test Where Square In Middle Of Tape Was Clicked", "[tape]") {
    const glm::vec2 kClickedPoint = glm::vec2(21, 11);
    const glm::vec2 kUpperCornerOfTape = glm::vec2(10, 10);
    const glm::vec2 kLowerCornerOfTape = glm::vec2(26, 15);
    REQUIRE(kHelperMethods.GetIndexOfSquareOfTapeClicked(kClickedPoint,
        kUpperCornerOfTape, kLowerCornerOfTape) == 5);
  }
}
