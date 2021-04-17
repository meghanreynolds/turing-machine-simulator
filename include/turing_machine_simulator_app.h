#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace turingmachinesimulator {

/**
 * App that simulates a Turing Machine created by users
 */
class TuringMachineSimulatorApp : public ci::app::App {
  public:
    /**
     * This method creates a new instance of the Turing Machine Simulator App
     */
    TuringMachineSimulatorApp();
};

} // namespace turingmachinesimulator
