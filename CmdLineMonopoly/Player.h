#pragma once

#define PDC_WIDE
#include <string>
#include <curses.h>

using namespace std;

constexpr int COLOR_SELECTION_START = 7;
constexpr int MAX_PLAYERS = 4;
constexpr int PLAYER_HEIGHT = 6;
constexpr int PLAYER_WIDTH = 20;

class Player
{
public:
  Player(unsigned char id);
  Player(Player&& p);
  ~Player();
  bool queryAttributes();
  unsigned char color;
  unsigned char boardItemIndex; // the index _inside_ the current board item which the player is on (useful when more than 1 player is on an item)
private:
  string name;
  unsigned char id;
  WINDOW* win;
  unsigned char static usedColors;
};
