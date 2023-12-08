#pragma once

#include <string>
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
  void showChanceDraw(string text, RandomDraw::RandomDrawType type);
private:
  WINDOW* win;
  WINDOW* diceWin;
};
