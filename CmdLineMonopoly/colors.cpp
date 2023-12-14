#include "colors.h"
#include <curses.h>

wchar_t *iconChars;

/// @brief Initialize all the colors for use with Curses
void init_colors() {
  start_color();
  // These are on a 0-100 instead of 0-255 scale
  init_color(COLOR_PURPLE, 376, 63, 294);
  init_color(COLOR_LBLUE, 459, 639, 1000);
  init_color(COLOR_PINK, 988, 216, 486);
  init_color(COLOR_ORANGE, 937, 490, 145);
  init_color(COLOR_RED_M, 867, 114, 71);
  init_color(COLOR_YELLOW_M, 918, 808, 176);
  init_color(COLOR_GREEN_M, 71, 761, 345);
  init_color(COLOR_BLUE_M, 0, 216, 847);

  init_pair(BGT_PURPLE, COLOR_WHITE, COLOR_PURPLE);
  init_pair(BGT_LBLUE, COLOR_BLACK, COLOR_LBLUE);
  init_pair(BGT_PINK, COLOR_WHITE, COLOR_PINK);
  init_pair(BGT_ORANGE, COLOR_BLACK, COLOR_ORANGE);
  init_pair(BGT_RED, COLOR_WHITE, COLOR_RED_M);
  init_pair(BGT_YELLOW, COLOR_BLACK, COLOR_YELLOW_M);
  init_pair(BGT_GREEN, COLOR_BLACK, COLOR_GREEN_M);
  init_pair(BGT_BLUE, COLOR_WHITE, COLOR_BLUE_M);
  init_pair(BGT_BLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(BGT_WHITE, COLOR_BLACK, COLOR_WHITE);

  init_pair(TXT_PURPLE, COLOR_PURPLE, COLOR_BLACK);
  init_pair(TXT_LBLUE, COLOR_LBLUE, COLOR_BLACK);
  init_pair(TXT_PINK, COLOR_PINK, COLOR_BLACK);
  init_pair(TXT_ORANGE, COLOR_ORANGE, COLOR_BLACK);
  init_pair(TXT_RED, COLOR_RED_M, COLOR_BLACK);
  init_pair(TXT_YELLOW, COLOR_YELLOW_M, COLOR_BLACK);
  init_pair(TXT_GREEN, COLOR_GREEN_M, COLOR_BLACK);
  init_pair(TXT_BLUE, COLOR_BLUE_M, COLOR_BLACK);
  init_pair(TXT_WHITE, COLOR_WHITE, COLOR_BLACK);
}

// Singleton object
Icons& Icons::icons() {
  static Icons icons;
  return icons;
}

Icons::Icons() {
  setcchar(&houseChar, iconChars, 0, TXT_GREEN, NULL);
  setcchar(&hotelChar, iconChars + 1, 0, TXT_RED, NULL);
}

cchar_t* Icons::house() {
  return &icons().houseChar;
}

cchar_t* Icons::hotel() {
  return &icons().hotelChar;
}
