#pragma once

#include <curses.h>
#include <string>
#include <random>
#include <vector>
#include "BoardItems.h"
#include "Player.h"

using namespace std;

constexpr char NUM_MENU_ITEMS = 6;

class BoardState
{
public:
  BoardState();
  ~BoardState();
  void drawInitial();
  void getPlayers();
  void handleCharInput(int ch);
  bool setYesNoPrompt(string prompt);
  
private:
  WINDOW* win;
  random_device rd;
  mt19937 mt;
  uniform_int_distribution<unsigned short> dice;
  char drawMenu(unsigned char playerId, string location);
  void doTurn(unsigned char playerId);
  unsigned char rollDice();
  vector<Player> players;

  Go go;
  Jail jail;
  FreeParking freeParking;
  GoToJail goToJail;
  TaxItem luxuryTax;
  TaxItem incomeTax;
  RandomDraw randomDrawItems[6];
  Property properties[28];
  BoardItem* boardItems[40];
};