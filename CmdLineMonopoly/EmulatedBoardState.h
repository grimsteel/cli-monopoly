#pragma once

#include "BoardState.h"
#include <string>
#include <vector>
#include <queue>

class EmulatedBoardState : public BoardState
{
public:
  EmulatedBoardState(std::string statePath);
  void getPlayers() override;
private:
  std::string statePath;
  unsigned short drawChance() override;
  unsigned short currentChanceIndex = 4;
  unsigned char rollDice(unsigned char playerId) override;
  vector<queue<pair<unsigned char, unsigned char>>> diceRolls;
};

