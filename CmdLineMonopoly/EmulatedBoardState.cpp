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
  unsigned short dice1;
  unsigned short dice2;

  while (getline(stateFile, line)) {
    istringstream values(line);
    
    values >> playerName >> playerBalance >> playerColor >> playerLocation;
    players.emplace_back(static_cast<unsigned char>(players.size()), playerName, playerBalance, static_cast<unsigned char>(playerLocation), static_cast<unsigned char>(playerColor));
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
      properties[propertyIndex].setHouses(static_cast<unsigned char>(numHouses));
    }

    getline(stateFile, line);
    istringstream dice(line);

    // Read in the dice rolls
    auto playerDiceRolls = &diceRolls.emplace_back();

    while (dice >> dice1) {
        dice >> dice2;

        playerDiceRolls->push({ static_cast<unsigned char>(dice1), static_cast<unsigned char>(dice2) });
    }
  }
  doupdate();
}

unsigned short EmulatedBoardState::drawChance() {
    unsigned short index = currentChanceIndex;
    if (++currentChanceIndex >= 16) currentChanceIndex -= 16;
    return index;
}

unsigned char EmulatedBoardState::rollDice(unsigned char playerId) {
    auto playerRolls = &diceRolls[playerId];
       
    // If we have no more predermined rolls, just generate them randomly
    if (playerRolls->size() == 0) return BoardState::rollDice(playerId);
    pair<unsigned char, unsigned char> roll = playerRolls->front();
    playerRolls->pop();

    return static_cast<unsigned char>((roll.first & 0b111) | ((roll.second & 0b111) << 3));
}