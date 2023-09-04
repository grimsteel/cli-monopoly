#pragma once

#include <string>
#define PDC_WIDE
#include <curses.h>
#include "BoardItems.h"
#include "colors.h"

using namespace std;

constexpr int COLOR_SELECTION_START = 7;
constexpr int MAX_PLAYERS = 4;
constexpr int PLAYER_HEIGHT = 6;
constexpr int PLAYER_WIDTH = 20;

class Player
{
public:
  Player(unsigned char id);
  ~Player();
  bool queryAttributes();
private:
  string name;
  unsigned char id;
  unsigned char color;
  WINDOW* win;
  unsigned char static usedColors;
};
