#include "BoardState.h"
#include <curses.h>
#include <string>
#include <random>
#include "BoardItems.h"
#include "utils.h"

using namespace std;

BoardState::BoardState() : mt(rd()), dice(1, 6) {
    win = newwin(10, 30, 5, H_PROPERTY_WIDTH * 2 + V_PROPERTY_WIDTH + PROPERTIES_PER_SIDE + 10);
}
BoardState::~BoardState() {
    delwin(win);
}
void BoardState::setPlayerName(string name) {
  wattron(win, A_BOLD | A_UNDERLINE);
  mvwprintw(win, 0, 0, "%s, it's your turn.", name.c_str());
  wattroff(win, A_BOLD | A_UNDERLINE);
}
void BoardState::setLocation(string name) {
  mvwprintw(win, 1, 0, "You are on %s.", name.c_str());
}
bool BoardState::setYesNoPrompt(string prompt) {
  return showYesNoPrompt(win, prompt, 0, 2);
}
unsigned char BoardState::rollDice() {
  auto roll1 = dice(mt);
  auto roll2 = dice(mt);

  // Each roll is from 1 to 6, which can be represented with 3 bits
  // This combines the two rolls into one byte. Roll 1 is the first 3 bytes, roll 2 is the next 3
  return static_cast<unsigned char>((roll1 & 0b111) | ((roll2 & 0b111) << 3));
}
