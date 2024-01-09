#include "EmulatedBoardState.h"
#include <fstream>
#include <string>

EmulatedBoardState::EmulatedBoardState(std::string statePath) : BoardState() {
  ifstream stateFile;
  stateFile.open(statePath);

  /*
    State File Format
    
    Repeat:
    PLAYER_NAME BALANCE COLOR LOCATION
    SPACE_SEPARATED_DICE_ROLLS
    
    Repeat:
    PROPERTY_INDEX OWNED_BY NUM_HOUSES

    Example:

    aaa 300 0 3
    34 66 45
    bbb 450 2 39
    12 11 16

    2 0 3
    3 0 4
    4 0 3
    14 1 255

    In this case, there are two players: aaa and bbb
    aaa has $300 and is on board item 3
    bbb has $450 and is on 39

    aaa will roll a 3+4, bbb will roll a 1+2, aaa will roll a 6+6, and so on

    Properties 2, 3, and 4 are owned by aaa and have 3, 4, and 3 houses
    Property 14 is owned by bbb and is mortgaged
  */
}