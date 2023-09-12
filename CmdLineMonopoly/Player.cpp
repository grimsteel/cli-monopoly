#define PDC_WIDE
#include <curses.h>
#include "Player.h"
#include "BoardItems.h"
#include "BoardState.h"
#include "colors.h"
#include "utils.h"
#include <bit>

using namespace std;

Player::Player(unsigned char id) : id(id) {
  /*
  |------|
  |1    2|
  |3    4|
  |------|
  */

  // Players 1 and 3 display on the right
  int x = (id % 2 == 1) ? H_PROPERTY_WIDTH : H_PROPERTY_WIDTH + PROPERTIES_PER_SIDE * V_PROPERTY_WIDTH - PLAYER_WIDTH - 1;
  // Players 1 and 2 display on the top
  int y = (id <= 2) ? V_PROPERTY_HEIGHT : V_PROPERTY_HEIGHT + PROPERTIES_PER_SIDE * H_PROPERTY_HEIGHT - PLAYER_HEIGHT - 1;
  win = newwin(PLAYER_HEIGHT, PLAYER_WIDTH, y, x);
  keypad(win, TRUE);

  refresh();
  wrefresh(win);
}

Player::Player(Player&& p) noexcept : id(p.id), win(p.win) {
  p.win = nullptr;
}

Player::~Player() {
  if (win != nullptr)
    delwin(win);
}

/// @returns Whether another player should be added
bool Player::queryAttributes(BoardState* boardState) {
  wattron(win, A_BOLD | A_UNDERLINE);
  wprintw(win, "Player %d:", id);
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
        currentIndex = (currentIndex + 1 + ctz(mask)) % 8;
      } else {
        // Find the next unused color to the left
        // we have to shift it by sizeof(long) - sizeof(char) to move all of the bits to the left, so clz will work
        unsigned long mask = ~(rotl(usedColors, sizeof(usedColors) * 8 - currentIndex) << (sizeof(unsigned long) * 8 - sizeof(usedColors) * 8));
        currentIndex = (currentIndex - 1 - clz(mask)) % 8;
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

  if (id < MAX_PLAYERS) {
    addAnother = showYesNoPrompt(win, boardState, "Add another?", 0, 3);
  }

  wclear(win);
  mvwadd_wch(win, 0, 0, &selectedColorChar);

  wattron(win, A_BOLD | A_UNDERLINE);
  mvwaddstr(win, 0, 2, playerName);
  wattroff(win, A_BOLD | A_UNDERLINE);

  mvwaddstr(win, 1, 0, "0 Properties");
  wattron(win, COLOR_PAIR(TXT_GREEN));
  mvwaddstr(win, 2, 0, "$0");
  wattroff(win, COLOR_PAIR(TXT_GREEN));

  wrefresh(win);

  name = playerName;
  color = TXT_PURPLE + currentIndex;

  return addAnother;
}

unsigned char Player::usedColors = 0b00000000;
