#define PDC_WIDE
#include <curses.h>
#include "Player.h"
#include "BoardItems.h"
#include "BoardState.h"
#include "colors.h"
#include "utils.h"

using namespace std;

Player::Player(unsigned char id) : id(id) {
  /*
  |------|
  |0    1|
  |2    3|
  |------|
  */

  // Players 0 and 2 display on the right
  int x = (id % 2 == 0) ? H_PROPERTY_WIDTH : H_PROPERTY_WIDTH + PROPERTIES_PER_SIDE * V_PROPERTY_WIDTH - PLAYER_WIDTH - 1;
  // Players 0 and 1 display on the top
  int y = (id <= 1) ? V_PROPERTY_HEIGHT : V_PROPERTY_HEIGHT + PROPERTIES_PER_SIDE * H_PROPERTY_HEIGHT - PLAYER_HEIGHT - 1;
  win = newwin(PLAYER_HEIGHT, PLAYER_WIDTH, y, x);
  keypad(win, TRUE);

  refresh();
  wrefresh(win);
}

Player::~Player() {
  if (win != nullptr)
    delwin(win);
}

bool Player::alterBalance(short value, string reason) {
  // Check for bankruptcy
  // TODO: Actually kick player if they go bankrupt
  if (value + balance <= 0) return false;

  setBalance(balance + value);

  // ncurses does not support mvwdeleteln???
  wmove(win, 3, 0);
  wdeleteln(win);
  wattron(win, COLOR_PAIR(value > 0 ? TXT_GREEN : TXT_RED));
  mvwprintw(win, 5, 0, "%+d (%s)", value, reason.c_str());
  wattroff(win, COLOR_PAIR(value > 0 ? TXT_GREEN : TXT_RED));

  wrefresh(win);
  
  return true;
}

void Player::setBalance(short newBalance) {
  balance = newBalance;
  wattron(win, COLOR_PAIR(TXT_GREEN));
  mvwprintw(win, 2, 0, "$%d", balance);
  wclrtoeol(win);
  wattroff(win, COLOR_PAIR(TXT_GREEN));
  wnoutrefresh(win);
}

short Player::getTotalValue() {
  short totalValue = 0;
  for (auto property : properties) {
    totalValue += property->prices.price + property->numHouses * property->prices.buildingPrice;
  }
  return totalValue;
}

/// @returns Whether another player should be added
bool Player::queryAttributes(BoardState* boardState) {
  wattron(win, A_BOLD | A_UNDERLINE);
  wprintw(win, "Player %d:", id + 1);
  wattroff(win, A_BOLD | A_UNDERLINE);
  mvwaddstr(win, 1, 0, "Name: ");
  
  wrefresh(win);

  curs_set(TRUE);
  echo();
  char playerName[15];
  wgetnstr(win, playerName, sizeof(playerName) / sizeof(char) - 1); // 1 for the null terminator
  
  noecho();
  mvwaddstr(win, 1, 6, playerName);
  mvwaddstr(win, 2, 0, "Color: ");
  cchar_t playerColorChar;
  cchar_t selectedColorChar;
  setcchar(&playerColorChar, L"\uf415", 0, 0, NULL);
  setcchar(&selectedColorChar, L"\uf4ff", 0, 0, NULL);
  for (int i = 0; i < NUM_TXT_COLORS; i++) {
    // AND it with an inverted color flag to remove the existing color, then OR it with our new color
    SET_CCHAR_COLOR(playerColorChar, i + TXT_PURPLE);
    if (usedColors & 1 << i) waddch(win, ' ');
    else wadd_wch(win, &playerColorChar);
  }
  
  int x = COLOR_SELECTION_START + ctz(~usedColors);
  int currentIndex;

  while (true) {
    // Update selectedColorChar to the appropriate color and print it
    currentIndex = x - COLOR_SELECTION_START;
    SET_CCHAR_COLOR(selectedColorChar, currentIndex + TXT_PURPLE);
    wmove(win, 2, x);
    wadd_wch(win, &selectedColorChar);
    wmove(win, 2, x); // we'll have moved one over, so move back
    
    wrefresh(win);
    
    int ch = wgetch(win);

    boardState->handleCharInput(ch);
    
    if (ch == KEY_ENTER || ch == '\n') break;
    else if (ch == KEY_LEFT || ch == KEY_RIGHT) {
      // Update playerColorChar to the correct color and print it (this visually unselects the current one)
      SET_CCHAR_COLOR(playerColorChar, currentIndex + TXT_PURPLE);
      wadd_wch(win, &playerColorChar);
      if (ch == KEY_RIGHT) {
        // Find the next unused color to the right
        unsigned long mask = ~rotr(usedColors, currentIndex + 1);
        currentIndex = (currentIndex + 1 + ctz(mask)) % NUM_TXT_COLORS;
      } else {
        // Find the next unused color to the left
        // we have to shift it by sizeof(long) - sizeof(char) to move all of the bits to the left, so clz will work
        unsigned long mask = ~(static_cast<unsigned long>(rotl(usedColors, static_cast<unsigned char>(sizeof(usedColors) * 8 - currentIndex))) << (sizeof(unsigned long) * 8 - sizeof(usedColors) * 8));
        currentIndex = (currentIndex - 1 - clz(mask)) % NUM_TXT_COLORS;
      }
      x = currentIndex + COLOR_SELECTION_START;
    }
  }

  curs_set(FALSE);

  // Display the selected color
  usedColors |= 1 << currentIndex;
  mvwadd_wch(win, 2, COLOR_SELECTION_START, &selectedColorChar);
  wclrtoeol(win);

  bool addAnother = false;

  if (id < MAX_PLAYERS - 1) {
    addAnother = showYesNoPrompt(win, boardState, "Add another?", 0, 3);
  }

  wclear(win);
  mvwadd_wch(win, 0, 0, &selectedColorChar);

  wattron(win, A_BOLD | A_UNDERLINE);
  mvwaddstr(win, 0, 2, playerName);
  wattroff(win, A_BOLD | A_UNDERLINE);

  mvwaddstr(win, 1, 0, "0 Properties");
  setBalance(1500);

  wrefresh(win);

  name = playerName;
  color = TXT_PURPLE + currentIndex;

  return addAnother;
}

unsigned char Player::usedColors = 0b00000000;
