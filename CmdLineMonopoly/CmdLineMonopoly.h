#pragma once

#include <string>
#include <curses.h>

#define PROPERTY_B(i, name, price, colorGroup) Property(i, name, price, colorGroup, Bottom)
#define PROPERTY_T(i, name, price, colorGroup) Property(i, name, price, colorGroup, Top)

using namespace std;

constexpr int BGT_PURPLE = 1;
constexpr int BGT_LBLUE = 2;
constexpr int BGT_PINK = 3;
constexpr int BGT_ORANGE = 4;
constexpr int BGT_RED = 5;
constexpr int BGT_YELLOW = 6;
constexpr int BGT_GREEN = 7;
constexpr int BGT_BLUE = 8;
constexpr int BGT_BLACK = 9;

// Each character is about 2x as tall as it is wide, so the height to width ratio is more like 16:11.
constexpr int V_PROPERTY_HEIGHT = 8;
constexpr int V_PROPERTY_WIDTH = 11;

constexpr int H_PROPERTY_WIDTH = 16;
constexpr int H_PROPERTY_HEIGHT = 5;

// "Price $" is 7 characters, and most of the prices are 3 digits
constexpr int AVG_PRICESTRING_WIDTH = 10;
// Width - avg price string width divided by 2 is the distance from the left to the start of the price string
constexpr int V_PRICESTRING_X = (V_PROPERTY_WIDTH - AVG_PRICESTRING_WIDTH + 1) / 2;
// right above the bottom border
constexpr int V_PRICESTRING_Y = V_PROPERTY_HEIGHT - 2;

constexpr int PROPERTIES_PER_SIDE = 9;

constexpr char CHANCE_DISPLAY[] = "   Chance";
constexpr char COMMUNITY_CHEST_DISPLAY[] = " Community    Chest";

enum BoardItemLocation {
  Bottom,
  Left,
  Top,
  Right
};

class BoardItem {
public:
  BoardItem(int index, string name, BoardItemLocation location);
  ~BoardItem();
  void initWindow();
  string name;
  virtual void drawInitial(string displayName) = 0;
  void redraw() {
    if (win != nullptr) {
      wclear(win);
      delwin(win);
    }
    initWindow();
  }
protected:
  WINDOW *win = nullptr;
  BoardItemLocation location;
  int index;
};

/// A single Property card on the map
class Property : public BoardItem {
public:
  Property(int index, string name, short price, unsigned char colorGroup, BoardItemLocation location);
  short price;
  short mortgagePrice;
  short housePrice;
  unsigned char ownedBy = 255; // 0-254 are player IDs, 255 is -1
  unsigned char numHouses = 0; // 5 houses equals 1 hotel
  void drawInitial(string displayName) override;
private:
  unsigned char colorGroup;
};

/// Chance or Community Chest
class RandomDraw : public BoardItem {
public:
  enum RandomDrawType {
    Chance,
    CommunityChest
  };
  RandomDraw(int index, RandomDrawType type, BoardItemLocation location);
  void drawInitial(string displayName) override;
private:
  RandomDrawType type;
};