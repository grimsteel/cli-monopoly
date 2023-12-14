#define PDC_WIDE

#include "BoardCenter.h"
#include "BoardItems.h"
#include "colors.h"
#include <curses.h>
#include <thread>

using namespace std;

constexpr int BOARD_CENTER_X = H_PROPERTY_WIDTH + PROPERTIES_PER_SIDE * V_PROPERTY_WIDTH / 2;
constexpr int BOARD_CENTER_Y = V_PROPERTY_HEIGHT + PROPERTIES_PER_SIDE * H_PROPERTY_HEIGHT / 2;
constexpr int BOARD_CENTER_WIDTH = 20;
constexpr int BOARD_CENTER_HEIGHT = 5;
constexpr int DICE_WIDTH = 11;
constexpr int DICE_HEIGHT = 3;

BoardCenter::BoardCenter() {
  win = newwin(BOARD_CENTER_HEIGHT, BOARD_CENTER_WIDTH, BOARD_CENTER_Y - BOARD_CENTER_HEIGHT / 2, BOARD_CENTER_X - BOARD_CENTER_WIDTH / 2);
  diceWin = derwin(win, DICE_HEIGHT, DICE_WIDTH, (BOARD_CENTER_HEIGHT - DICE_HEIGHT) / 2, (BOARD_CENTER_WIDTH - DICE_WIDTH) / 2);
}

BoardCenter::~BoardCenter() {
  delwin(win);
  delwin(diceWin);
}

void BoardCenter::showDiceRoll(unsigned char roll1, unsigned char roll2) {
  wclear(win);
  wrefresh(win);
  wattron(diceWin, COLOR_PAIR(BGT_WHITE));
  mvwaddwstr(diceWin, 0, 0, L"┌───┐ ┌───┐│   │ │   │└───┘ └───┘");
  wattroff(diceWin, COLOR_PAIR(BGT_WHITE));
  // clear the characters between the two dice
  mvwvline(diceWin, 0, DICE_WIDTH / 2, ' ', DICE_HEIGHT);
  wattron(diceWin, COLOR_PAIR(BGT_WHITE));
  for (char currentDigit = '1'; currentDigit <= '6'; currentDigit++) {
    mvwaddch(diceWin, 1, 2, currentDigit);
    mvwaddch(diceWin, 1, 8, '6' - currentDigit + '1');

    wrefresh(diceWin);

    std::this_thread::sleep_for(100ms);
  }
  mvwaddch(diceWin, 1, 2, roll1 + '0');
  mvwaddch(diceWin, 1, 8, roll2 + '0');
  wattroff(diceWin, COLOR_PAIR(BGT_WHITE));

  wrefresh(diceWin);
}

void BoardCenter::showChanceDraw(char text[3][19], RandomDraw::RandomDrawType type) {
  chtype bgStyle = COLOR_PAIR(type == RandomDraw::RandomDrawType::Chance ? BGT_ORANGE : BGT_YELLOW);
  wclear(win);
  wclear(diceWin);

  wattron(win, bgStyle);
  mvwaddwstr(win, 0, 0, L"┌──────────────────┐│                  ││                  ││                  │└──────────────────┘");
  for (int i = 0; i < 3; i++) {
    mvwaddstr(win, i + 1, (BOARD_CENTER_WIDTH - static_cast<int>(strlen(text[i]))) / 2, text[i]);
  }
  wattroff(win, bgStyle);

  wrefresh(win);
}