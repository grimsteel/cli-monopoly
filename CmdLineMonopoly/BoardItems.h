#pragma once

#define PDC_WIDE

#include "Player.h"
#include <string>
#include <curses.h>

#define V_WIN(y, x) newwin(V_PROPERTY_HEIGHT, V_PROPERTY_WIDTH, y, x);
#define H_WIN(y, x) newwin(H_PROPERTY_HEIGHT, H_PROPERTY_WIDTH, y, x);
#define C_WIN(y, x) newwin(V_PROPERTY_HEIGHT, H_PROPERTY_WIDTH, y, x);

using namespace std;

// Each character is about 2x as tall as it is wide, so the height to width ratio is more like 16:11.
constexpr int V_PROPERTY_HEIGHT = 8;
constexpr int V_PROPERTY_WIDTH = 11;

constexpr int H_PROPERTY_WIDTH = 16;
constexpr int H_PROPERTY_HEIGHT = 5;

constexpr int PROPERTIES_PER_SIDE = 9;

// Forward declaration
class BoardState;

// We need to do -1 because the top border of the top left/right properties
// are on the same row as the bottom border of the top properties
// same reason for the right offset
constexpr int BOTTOM_OFFSET = V_PROPERTY_HEIGHT + H_PROPERTY_HEIGHT * PROPERTIES_PER_SIDE - 1;
constexpr int RIGHT_OFFSET = H_PROPERTY_WIDTH + PROPERTIES_PER_SIDE * V_PROPERTY_WIDTH - 1;

constexpr int JAIL_HEIGHT = 3;
constexpr int JAIL_WIDTH = 6;
constexpr int JAIL_X = H_PROPERTY_WIDTH - JAIL_WIDTH - 2;

constexpr char CHANCE_DISPLAY_V[] = "   Chance";
constexpr char COMMUNITY_CHEST_DISPLAY_V[] = " Community    Chest";
constexpr char CHANCE_DISPLAY_H[] = "     Chance";
constexpr char COMMUNITY_CHEST_DISPLAY_H[] = "   Community         Chest";
constexpr char INCOME_TAX_DISPLAY_V[] = "   Income      Tax";
constexpr char LUXURY_TAX_DISPLAY_H[] = "   Luxury Tax";

class BoardItem {
public:
  enum BoardItemLocation {
    Bottom = 1,
    Top = 2,
    Left = 4,
    Right = 8
  };
  inline friend BoardItemLocation operator |(BoardItemLocation a, BoardItemLocation b) {
    return static_cast<BoardItemLocation>(static_cast<int>(a) | static_cast<int>(b));
  }
  BoardItem(unsigned char index, string name, BoardItemLocation location);
  ~BoardItem();
  void initWindow();
  string name;
  /// @brief Draw this entire item
  /// Pure virtual
  virtual void drawInitial() = 0;
  virtual void handlePlayer(Player* player, BoardState* mainMenu);
  void handlePlayerLeave(unsigned char playerId);
  void redraw();
protected:
  WINDOW* win = nullptr;
  BoardItemLocation location;
  unsigned char index;
  unsigned char numPlayers = 0;
  /// @brief Y index of the list of players
  unsigned char playerListY;
};

/// A single Property card on the map
class Property : public BoardItem {
public:
  struct Prices {
    short price;
    // mortgage price is half of the normal price
    // unmortgage price is 1.1 * mortgageValue
    short buildingPrice;
    // initial rent, rent for 1-4 houses, and hotel rent
    short rent[6];
  };

  Property(unsigned char index, string name, string displayName, Prices prices, unsigned char colorGroup, BoardItemLocation location);
  string displayName;
  Prices prices;
  unsigned char ownedBy = 255; // 0-254 are player IDs, 255 is -1
  unsigned char numHouses = 0; // 5 houses equals 1 hotel
  void drawInitial() override;
  void handlePlayer(Player* player, BoardState* mainMenu) override;
private:
  unsigned char colorGroup;
  void drawPlayerOwn(Player* player);
};

/// Chance or Community Chest
class RandomDraw : public BoardItem {
public:
    enum RandomDrawType {
        Chance,
        CommunityChest
    };
    RandomDraw(unsigned char index, RandomDrawType type, BoardItemLocation location);
    void drawInitial() override;
private:
    RandomDrawType type;
};

/// Luxury/Income tax
class TaxItem : public BoardItem {
public:
    enum TaxType {
        Income,
        Luxury
    };
    TaxItem(unsigned char index, TaxType type, BoardItemLocation location);
    void drawInitial() override;
private:
    TaxType type;
};

/// Four corners
class Go : public BoardItem {
public:
  Go();
  void drawInitial() override;
};

class Jail : public BoardItem {
public:
  Jail();
  void drawInitial() override;
};

class FreeParking : public BoardItem {
public:
  FreeParking();
  void drawInitial() override;
  void fixBorder();
};

class GoToJail : public BoardItem {
public:
  GoToJail();
  void drawInitial() override;
  void fixBorder();
};