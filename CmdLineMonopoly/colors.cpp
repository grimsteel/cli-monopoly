#include "colors.h"
#include <curses.h>

/// @brief Initialize all the colors for use with Curses
void init_colors() {
  start_color();
  init_pair(BGT_PURPLE, COLOR_WHITE, COLOR_MAGENTA | 0b1000); // black on light magenta (somehow this looks purple)
  init_pair(BGT_LBLUE, COLOR_BLACK, COLOR_CYAN | 0b1000); // black on light cyan
  init_pair(BGT_PINK, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(BGT_ORANGE, COLOR_BLACK, COLOR_RED | 0b1000); // black on light red (orange?)
  init_pair(BGT_RED, COLOR_WHITE, COLOR_RED);
  init_pair(BGT_YELLOW, COLOR_BLACK, COLOR_YELLOW);
  init_pair(BGT_GREEN, COLOR_BLACK, COLOR_GREEN);
  init_pair(BGT_BLUE, COLOR_WHITE, COLOR_BLUE);
  init_pair(BGT_BLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(BGT_WHITE, COLOR_BLACK, COLOR_WHITE);

  init_pair(TXT_PURPLE, COLOR_MAGENTA | 0b1000, COLOR_BLACK);
  init_pair(TXT_PINK, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(TXT_ORANGE, COLOR_RED | 0b1000, COLOR_BLACK);
  init_pair(TXT_RED, COLOR_RED, COLOR_BLACK);
  init_pair(TXT_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(TXT_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(TXT_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(TXT_WHITE, COLOR_WHITE, COLOR_BLACK);
}
