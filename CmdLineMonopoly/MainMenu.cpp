#include "MainMenu.h"
#include <curses.h>
#include <string>
#include "BoardItems.h"
#include "utils.h"

using namespace std;

MainMenu::MainMenu() {
    win = newwin(10, 30, 5, H_PROPERTY_WIDTH * 2 + V_PROPERTY_WIDTH + PROPERTIES_PER_SIDE + 10);
}
MainMenu::~MainMenu() {
    delwin(win);
}
void MainMenu::setPlayerName(string name) {
  wattron(win, A_BOLD | A_UNDERLINE);
  mvwprintw(win, 0, 0, "It is %s's turn.", name.c_str());
  wattroff(win, A_BOLD | A_UNDERLINE);
}
void MainMenu::setLocation(string name) {
  mvwprintw(win, 1, 0, "You landed on %s.", name.c_str());
}
bool MainMenu::setYesNoPrompt(string prompt) {
  return showYesNoPrompt(win, prompt, 0, 2);
}