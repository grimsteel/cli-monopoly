#pragma once

#define PDC_WIDE
#include <string>
#include <curses.h>

// Forward declaration
class BoardState;

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
  bool queryAttributes(BoardState* boardState);
  unsigned char color = 0;
  // Every player starts on go
  unsigned char boardItemIndex = 0;
  unsigned int balance = 1500;
  unsigned char id;
  string name;
private:
  WINDOW* win;
  unsigned char static usedColors;
};
