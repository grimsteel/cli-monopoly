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
  void showChanceDraw(int i, RandomDraw::RandomDrawType type);
  vector<Player> players;
  mt19937 mt;
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
  uniform_int_distribution<unsigned short> dice;
  void drawHeader(unsigned char playerId, string location);
  void drawSubheader(string text);
  vector<Property*> promptChooseProperty(vector<unsigned char> chooseFrom, bool onlyPrintProperties, unsigned char ownedBy);
  NavigateListResult navigateList(unsigned char maxItems, unsigned char currentItem);
  bool updateManageHousesStats(short totalNewHouses, short buildingPrice, short currentMoney, vector<unsigned char> newHouses);
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
  char chanceMessages[16][3][19] = {
      { "Advance to", "Boardwalk" },
      { "Advance to Go", "Collect $200" },
      { "Advance to", "Illinois Avenue", "(Can collect $200)" },
      { "Advance to", "St. Charles Place", "(Can collect $200)" },
      { "Advance to nearest", "railroad. Buy or", "pay 2x rent" },
      { "Advance to nearest", "railroad. Buy or", "pay 2x rent" },
      { "Advance to nearest", "utility. Buy or", "pay 10x dice throw" },
      { "Bank pays you", "dividend of $50" },
      { "Get Out of", "Jail Free" },
      { "Go to jail.", "Do not pass Go.", "Don't collect $200" },
      { "Go back", "three spaces" },
      { "Make repairs.", "Pay $25 per house", "Pay $100 for hotel" },
      { "Speeding fine", "Pay $15" },
      { "Take a trip to", "Reading Railroad", "(Can collect $200)" },
      { "You were elected", "Chairman of Board", "Pay $50 to all" },
      { "Your building", "loan matures", "Collect $150." }
  };
  char communityChest[16][3][19];
};