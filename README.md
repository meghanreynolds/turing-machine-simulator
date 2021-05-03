# Turing Machine Simulator
<i>This goal of this project is to allow people to explore Turing Machines by creating their own
and seeing how they would work in practice. People can create a State Diagrams representing Turning
Machines and set the tape of the machine. After clicking simulate, the simulation will update the tape according to the 
rules entered in via the state diagram, moving the scanner along the tape as a Turning Machine would.</i>

## Features ##

###### Drag and Drop States From The Menu Onto The Canvas ######
``` GIF displaying this feature will go here next week```

###### Delete States By Dragging Them Back Onto The Menu States ######
``` GIF displaying this feature will go here next week```

###### Add Arrows Between States Using The Add Arrow Box ######
``` GIF displaying this feature will go here next week```

###### Set The Tape By Clicking On The Squares And Typing Any Character (Return Once Finished Editing) ######
``` GIF displaying this feature will go here next week```

###### Rename Nodes By Right Clicking On Them (Return Once Finished Editing) ######
``` GIF displaying this feature will go here next week```

###### Simulate The Turing Machine ######
``` GIF displaying this feature will go here next week```

###### View The Complete Configuration Of The Turing Machine Simulation ######
``` GIF displaying this feature will go here next week```

###### Reset The Tape To Re-Run The Simulation ######
``` GIF displaying this feature will go here next week```

###### Terminate The Simulation By Clicking The Stop Simulation Button ######
``` GIF displaying this feature will go here next week```

###### Clear Out The Canvas To Create A New Turing Machine ######
``` GIF displaying this feature will go here next week```

## Quick Start (MacOS) ##
1. Download CMake
   ```
      $ brew install cmake
   ```
1. Download Cinder from https://libcinder.org/download
2. Create a folder in the Cinder Folder
3. Clone this repository into the folder you created:
    ```
    git clone https://github.com/uiuc-sp21-cs126/final-project-meghanreynolds.git
    ```
4. Set the run configuration of turing-machine-simulator to have the following working directory: 
    ```
       $ProjectFileDir$
    ```

## Notation, Assumptions, and Conventions ##
The notation that will be used in this simulation is as follows:
- **q<sub>1</sub>:** the starting state
- **q<sub>n</sub>:** the nth state
- **q<sub>h</sub>, q<sub>Accept</sub>, q<sub>Reject</sub>:** the halting states
- **L:** move the scanner 1 square to the left
- **R:** move the scanner 1 square to the right
- **N:** do not move the scanner
- **'-':** blank square
- Rules will be written on arrows as 'read, write, move'.
  >'-, 1, N' reads as <i> "Read blank, Write 1, Do
  not move"</i>  
  > '0, 0, R' <i>reads as "Read 0, Write 0, Move Right 1 Square"</i>

The simulation will follow the following conventions and assumptions:
- The scanner will initially be placed on the 1st square of the tape
- The machine will start in q<sub>1</sub>
- There exist infinite blank squares to either side of the user input on the tape
- When displaying complete configurations, blanks will be displayed once they have been scanned at least one time (or
  if they were on the starting tape)
  