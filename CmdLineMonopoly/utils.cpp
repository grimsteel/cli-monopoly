#include "utils.h"
#include "colors.h"
#include <curses.h>
#include <string>

bool showYesNoPrompt(WINDOW* win, std::string prompt, int x, int y) {
  unsigned char result = 1;
  mvwaddstr(win, y, x, prompt.c_str());
  while (true) {
    wattron(win, COLOR_PAIR(BGT_BLACK + result)); // If result is 1, then this makes it white on black
    mvwaddstr(win, y, x + static_cast<int>(prompt.length()) + 1, "Yes");
    wattroff(win, COLOR_PAIR(BGT_BLACK + result));
    wattron(win, COLOR_PAIR(BGT_WHITE - result)); // If result is 1, this makes it black on white
    mvwaddstr(win, y, x + static_cast<int>(prompt.length()) + 5, "No");
    wattroff(win, COLOR_PAIR(BGT_WHITE - result));
    wrefresh(win);
    
    int ch = wgetch(win);
    if (ch == KEY_ENTER || ch == '\n') break;
    else if (ch == KEY_LEFT || ch == KEY_RIGHT) result = !result;
  }
  wmove(win, y, x);
  wclrtoeol(win);

  return result == 1;
}