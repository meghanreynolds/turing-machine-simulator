# Turing Machine Simulator
<i>This goal of this project is to allow people to explore Turing Machines by creating their own
and seeing how they would work in practice. People can create a State Diagrams representing Turning
Machines and set the tape of the machine. After clicking simulate, the simulation will update the tape according to the 
rules entered in via the state diagram, moving the scanner along the tape as a Turning Machine would. People can change
the Turing Machine's blank square character and the simulation mode (step through or automatic) in settings. The
complete configurations of the simulations will be outputted to the complete_configuration.md file in the 
complete-configuration folder or the terminal (if the file is unable to be found).</i>

## Features ##

###### Drag and Drop States From The Menu Onto The Canvas ######
![Drag And Drop States](https://media.giphy.com/media/nppKbAdUh3Zy4LsDCg/giphy.gif)

###### Delete States By Dragging Them Back Onto The Menu States ######
![Delete States](https://media.giphy.com/media/JlhtoAdzgSALnJI873/giphy.gif)

###### Rename States By Right Clicking On Them (Return Once Finished Editing) ######
![Rename States](https://media.giphy.com/media/WTSNRefGh07v1tGpSk/giphy.gif)

###### Add Arrows Between States Using The Add Arrow Box ######
![Add Arrows](https://media.giphy.com/media/EFp4usuSBYnIQmQ5aL/giphy.gif)

###### Accept And Reject Halting States ######
![Accept And Reject](https://media.giphy.com/media/SUhzfwftU0a9njfozq/giphy.gif)

###### Set The Tape By Clicking On The Squares And Typing Any Character (Return Once Finished Editing) ######
![Set Tape](https://media.giphy.com/media/D8RcO3UpVBBKnsrhgR/giphy.gif)

###### Select The Default Blank Character ######
![Set Blank Character](https://media.giphy.com/media/wsE0AEg28b6YLSAvQA/giphy.gif)

###### Simulate The Turing Machine ######
![Simulation](https://media.giphy.com/media/Sc5ZQlJxz2m0Mtwt0w/giphy.gif)

###### Terminate The Simulation By Clicking The Stop Simulation Button ######
![Stop Simulation](https://media.giphy.com/media/8x3VCUsBwkDm2AjXBD/giphy.gif)

###### Step Through The Simulation ######
![Step Through Simulation](https://media.giphy.com/media/LVRprcnX25Puq1WuzM/giphy.gif)

###### Reset The Tape To Re-Run The Simulation ######
![Reset Tape](https://media.giphy.com/media/3uwNi02Eoe8nBrWEU4/giphy.gif)

###### Output The Complete Configuration Of The Turing Machine Simulation To A Markdown File ######
![Markdown Configuration](https://media.giphy.com/media/3H8jdnKJsw3L2m01vh/giphy.gif)

###### Output The Complete Configuration Of The Turing Machine Simulation To The Console ######
![Console Configuration](https://media.giphy.com/media/rtuUgdlvWoFqhlzOz7/giphy.gif)

###### Clear Out The Canvas To Create A New Turing Machine ######
![Clear](https://media.giphy.com/media/stgk5eO3mFcH6urZA9/giphy.gif)

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
- **'-':** default blank square (can be changed in settings)
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
  