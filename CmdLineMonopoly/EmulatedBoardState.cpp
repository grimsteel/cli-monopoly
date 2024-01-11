#include "EmulatedBoardState.h"
#include "BoardItems.h"
#include "BoardState.h"
#include "Player.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>

EmulatedBoardState::EmulatedBoardState(std::string statePath) : BoardState(), statePath(statePath) { }

void EmulatedBoardState::getPlayers() {
  ifstream stateFile;
  stateFile.open(statePath);

  BoardItem::RollInfo dummyRollInfo = {
    .roll = 0,
    .isChanceMultiplied = false
  };

  string line;

  string playerName;
  short playerBalance;
  unsigned short playerColor;
  unsigned short playerLocation;
  unsigned short propertyIndex;
  unsigned short numHouses;

  while (getline(stateFile, line)) {
    istringstream values(line);
    
    values >> playerName >> playerBalance >> playerColor >> playerLocation;
    players.emplace_back(players.size() + 1, playerName, playerBalance, playerLocation, playerColor);
    Player *newPlayer = &(*(players.end() - 1));
    
    // Render the player
    boardItems[newPlayer->boardItemIndex]->handlePlayer(newPlayer, this, &dummyRollInfo);

    getline(stateFile, line);
    values.str(line);
    values.seekg(0);
    
    while (values >> propertyIndex) {
      values >> numHouses;

      properties[propertyIndex].drawPlayerOwn(newPlayer);
      newPlayer->addProperty(&properties[propertyIndex]);
      properties[propertyIndex].setHouses(numHouses);
    }
  }
  doupdate();
}


  /*
    State File Format
    
    Repeat:
    PLAYER_NAME BALANCE COLOR LOCATION
    PROPERTY_INDEX NUM_HOUSES (repeated)
    SPACE_SEPARATED_DICE_ROLLS

    Example:

    aaa 300 0 3
    2 3 3 4 4 3
    3 4 6 6 4 5
    bbb 450 2 39
    14 255
    12 11 16

    In this case, there are two players: aaa and bbb
    aaa has $300 and is on board item 3
    bbb has $450 and is on 39

    aaa will roll a 3+4, bbb will roll a 1+2, aaa will roll a 6+6, and so on

    Properties 2, 3, and 4 are owned by aaa and have 3, 4, and 3 houses
    Property 14 is owned by bbb and is mortgaged
  */
