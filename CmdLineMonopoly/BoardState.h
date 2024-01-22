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
  virtual void getPlayers();
  void mainLoop();
  void handleCharInput(int ch);
  unsigned char numRailroadsOwned(unsigned char playerId);
  bool ownsBothUtilities(unsigned char playerId);
  bool setYesNoPrompt(string prompt);
  void showChanceDraw(unsigned char playerId, RandomDraw::RandomDrawType type, BoardItem::RollInfo *rollInfo);
  vector<Player> players;

  void run() {
    drawInitial();
    getPlayers();
    mainLoop();
  }
protected:
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
  uniform_int_distribution<unsigned short> chanceDrawer;
  void drawHeader(unsigned char playerId, string location);
  void drawSubheader(string text);
  vector<Property*> promptChooseProperty(vector<unsigned char> chooseFrom, bool onlyPrintProperties, unsigned char ownedBy);
  NavigateListResult navigateList(unsigned char maxItems, unsigned char currentItem);
  bool updateManageHousesStats(short totalNewHouses, short buildingPrice, short currentMoney, vector<unsigned char> newHouses);
  char drawMenu(bool showRollDice);
  /// <returns>Boolean - true if the game should be quit</returns>
  bool doTurn(unsigned char playerId);
  virtual unsigned char rollDice(unsigned char playerId);
  void movePlayerTo(unsigned char playerId, unsigned char boardItemIndex, BoardItem::RollInfo* rollInfo);
  virtual unsigned short drawChance();

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
    { "Advance to", "Illinois Avenue" },
    { "Advance to", "St. Charles Place" },
    { "Advance to nearest", "railroad. Buy or", "pay 2x rent" },
    { "Advance to nearest", "railroad. Buy or", "pay 2x rent" },
    { "Advance to nearest", "utility. Buy or", "pay 10x dice throw" },
    { "Bank pays you", "dividend of $50" },
    { "Get out of", "jail free." },
    { "Go to jail.", "Do not pass Go.", "Don't collect $200" },
    { "Go back", "three spaces" },
    { "Make repairs.", "Pay $25 per house", "Pay $100 for hotel" },
    { "Pay poor tax", "of $15" },
    { "Take a ride on", "the Reading.", "(Can collect $200)" },
    { "You were elected", "Chairman of Board", "Pay $50 to each" },
    { "Your building", "loan matures", "Collect $150." }
  };
  char communityChestMessages[16][3][19] = {
    { "Life insurance", "matures", "Collect $100." },
    { "From sale of stock", "you get $45." },
    { "Xmas fund matures.", "Collect $100" },
    { "Pay hospital", "$100." },
    { "Get out of", "jail free." },
    { "Pay school tax", "of $150." },
    { "Go to jail.", "Do not pass Go.", "Don't collect $200" },
    { "Advance to Go", "Collect $200" },
    { "Grand opera", "opening. Collect", "$50 from each." },
    { "Income tax refund", "Collect $20." },
    { "Receive for", "services $25." },
    { "You inherit", "$100." },
    { "Bank error in", "your favor.", "Collect $200." },
    { "You won 2nd place", "in beauty contest", "Collect $10."},
    { "Doctor's fee.", "Pay $50" },
    { "Street repairs:", "Houses: $40", "Hotels: $115" }
  };
};