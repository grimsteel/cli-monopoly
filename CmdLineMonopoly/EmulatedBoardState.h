#pragma once

#include "BoardState.h"
#include <string>
#include <vector>

class EmulatedBoardState : public BoardState
{
public:
  EmulatedBoardState(std::string statePath);
  void getPlayers() override;
private:
  std::string statePath;
};

