#include "BoardCenter.h"
#include "BoardItems.h"
#define PDC_WIDE
#include <curses.h>

BoardCenter::BoardCenter() {
  constexpr int BOARD_CENTER_X = H_PROPERTY_WIDTH + PROPERTIES_PER_SIDE * V_PROPERTY_WIDTH / 2;
  constexpr int BOARD_CENTER_Y = V_PROPERTY_HEIGHT + PROPERTIES_PER_SIDE * H_PROPERTY_HEIGHT / 2;
  win = newwin(10, 10, BOARD_CENTER_Y, BOARD_CENTER_X);
}

BoardCenter::~BoardCenter() {
  delwin(win);
}

void BoardCenter::showDiceRoll(unsigned char roll1, unsigned char roll2) {
  mvwaddwstr(win, 0, 0, L"┌───┐\n│   │\n└───┘");
  mvwaddch(win, 2, 1, roll1 + 48);
  //mvwprintw(win, 0, 0, L"┌───┐\n│ %d │\n└───┘\n%d", roll1, roll2);
  wnoutrefresh(win);
}