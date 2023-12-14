#pragma once

#include <curses.h>
#include "BoardItems.h"

using namespace std;

class BoardCenter
{
public:
  BoardCenter();
  ~BoardCenter();
  void showLogo();
  void showDiceRoll(unsigned char roll1, unsigned char roll2);
  void showChanceDraw(char text[3][19], RandomDraw::RandomDrawType type);
private:
  WINDOW* win;
  WINDOW* diceWin;
};
