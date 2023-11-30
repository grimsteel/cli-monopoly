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
  enum NavigateListResult {
    Up = -1,
    Confirm,
    Down,

    Cancel,
    NoAction,

    Left,
    Right
  };

  WINDOW* win;
  BoardCenter boardCenter;
  random_device rd;
  mt19937 mt;
  uniform_int_distribution<unsigned short> dice;
  void drawHeader(unsigned char playerId, string location);
  void drawSubheader(string text);
  vector<Property*> promptChooseProperty(vector<unsigned char> chooseFrom, bool onlyPrintProperties);
  NavigateListResult navigateList(unsigned char maxItems, unsigned char currentItem);
  bool updateManageHousesStats(short totalNewHouses, short buildingPrice, vector<unsigned char> newHouses);
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