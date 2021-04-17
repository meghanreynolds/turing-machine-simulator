#include "turing_machine_simulator_app.h"

using namespace turingmachinesimulator;

void prepareSettings(TuringMachineSimulatorApp::Settings* settings) {
  settings->setResizable(false);
}

CINDER_APP(TuringMachineSimulatorApp, ci::app::RendererGl, prepareSettings);
