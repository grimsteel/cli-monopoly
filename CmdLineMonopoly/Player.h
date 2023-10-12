#pragma once

#define PDC_WIDE
#include <string>
#include <curses.h>
#include <vector>

// Forward declaration
class BoardState;
class Property;

using namespace std;

constexpr int COLOR_SELECTION_START = 7;
constexpr int MAX_PLAYERS = 4;
constexpr int PLAYER_HEIGHT = 6;
constexpr int PLAYER_WIDTH = 40;

class Player
{
public:
  Player(unsigned char id);
  ~Player();
  bool queryAttributes(BoardState* boardState);
  unsigned char color = 0;
  // Every player starts on go
  unsigned char boardItemIndex = 0;
  unsigned char id;
  string name;
  short getBalance() { return balance; }
  bool alterBalance(short value, string reason);
  void addProperty(Property* property) { properties.push_back(property); }
private:
  short balance = 0;
  WINDOW* win;
  unsigned char static usedColors;
  vector<Property*> properties;
  void setBalance(short newBalance);
};
