#pragma once

#include <curses.h>
#include <string>
#include <random>
#include <vector>
#include "BoardItems.h"
#include "BoardCenter.h"
#include "Player.h"

using namespace std;

constexpr char NUM_MENU_ITEMS = 6;

// Correspond to indices 5, 15, 25, and 35 in the boardItems array
constexpr int railroadPropertyIndices[] = { 2, 10, 17, 25 };

constexpr int utilityPropertyIndices[] = { 7, 20 };

class BoardState
{
public:
  BoardState();
  ~BoardState();
  void drawInitial();
  void getPlayers();
  void mainLoop();
  void handleCharInput(int ch);
  unsigned char numRailroadsOwned(unsigned char playerId);
  bool ownsBothUtilities(unsigned char playerId);
  bool setYesNoPrompt(string prompt);
  vector<Player> players;
private:
  WINDOW* win;
  BoardCenter boardCenter;
  random_device rd;
  mt19937 mt;
  uniform_int_distribution<unsigned short> dice;
  void drawHeader(unsigned char playerId, string location);
  char drawMenu(bool showRollDice);
  /// <returns>Boolean - true if the game should be quit</returns>
  bool doTurn(unsigned char playerId);
  unsigned char rollDice();

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