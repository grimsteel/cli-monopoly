#define PDC_WIDE

#include "BoardCenter.h"
#include "BoardItems.h"
#include "colors.h"
#include <curses.h>
#include <thread>

constexpr int BOARD_CENTER_X = H_PROPERTY_WIDTH + PROPERTIES_PER_SIDE * V_PROPERTY_WIDTH / 2;
constexpr int BOARD_CENTER_Y = V_PROPERTY_HEIGHT + PROPERTIES_PER_SIDE * H_PROPERTY_HEIGHT / 2;
constexpr int BOARD_CENTER_WIDTH = 11;
constexpr int BOARD_CENTER_HEIGHT = 3;

BoardCenter::BoardCenter() {
  win = newwin(BOARD_CENTER_HEIGHT, BOARD_CENTER_WIDTH, BOARD_CENTER_Y - BOARD_CENTER_HEIGHT / 2, BOARD_CENTER_X - BOARD_CENTER_WIDTH / 2);
}

BoardCenter::~BoardCenter() {
  delwin(win);
}

void BoardCenter::showDiceRoll(unsigned char roll1, unsigned char roll2) {
  wclear(win);
  wattron(win, COLOR_PAIR(BGT_WHITE));
  mvwaddwstr(win, 0, 0, L"┌───┐ ┌───┐│   │ │   │└───┘ └───┘");
  wattroff(win, COLOR_PAIR(BGT_WHITE));
  // clear the characters between the two dice
  mvwvline(win, 0, BOARD_CENTER_WIDTH / 2, ' ', BOARD_CENTER_HEIGHT);
  wattron(win, COLOR_PAIR(BGT_WHITE));
  for (char currentDigit = '1'; currentDigit <= '6'; currentDigit++) {
    mvwaddch(win, 1, 2, currentDigit);
    mvwaddch(win, 1, 8, '6' - currentDigit + '1');
    wrefresh(win);
    std::this_thread::sleep_for(100ms);
  }
  mvwaddch(win, 1, 2, roll1 + '0');
  mvwaddch(win, 1, 8, roll2 + '0');
  wattroff(win, COLOR_PAIR(BGT_WHITE));
  wrefresh(win);
}