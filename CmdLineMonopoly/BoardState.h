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
  void drawSubheader(string text);
  vector<Property*> promptChooseProperty(vector<unsigned char> chooseFrom, bool onlyPrintProperties);
  /// <summary>
  /// Evaluate the input for navigating a list
  /// </summary>
  /// <returns>-1 if the user is moving up, 1 if they are moving down, 0 if they confirmed, 2 if they're cancelling, -2 if no action</returns>
  int navigateList(unsigned char maxItems, unsigned char currentItem);
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