#include "PlayerStats.h"
#include "BoardItems.h"
#include <curses.h>

PlayerStats::PlayerStats() {
  win = newwin(6, 20, V_PROPERTY_HEIGHT, H_PROPERTY_WIDTH);
}

PlayerStats::~PlayerStats() {
  delwin(win);
}

void PlayerStats::getAttributes() {
  wattron(win, A_BOLD | A_UNDERLINE);
  wprintw(win, "Player 1:");
  wattroff(win, A_BOLD | A_UNDERLINE);
  mvwprintw(win, 1, 0, "Name: ");
  
  wrefresh(win);

  curs_set(TRUE);
  echo();
  char playerName[15];
  wgetnstr(win, playerName, sizeof(playerName) / sizeof(char) - 1); // 1 for the null terminator
  mvwprintw(win, 1, 6, playerName);
  mvwprintw(win, 2, 0, "Color: ");
  wrefresh(win);
  curs_set(FALSE);

  noecho();
  name = playerName;
}