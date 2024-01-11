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
    players.emplace_back(static_cast<unsigned char>(players.size() + 1), playerName, playerBalance, static_cast<unsigned char>(playerLocation), static_cast<unsigned char>(playerColor));
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
