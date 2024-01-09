#pragma once

#include "CmdLineMonopoly.h"
#include "BoardState.h"
#include <string>

class EmulatedBoardState : BoardState
{
public:
  EmulatedBoardState(std::string statePath);
};

