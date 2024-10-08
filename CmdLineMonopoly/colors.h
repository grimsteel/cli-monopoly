#pragma once
#define PDC_WIDE

#include <curses.h>

void init_colors();

constexpr int COLOR_PURPLE = 16;
constexpr int COLOR_LBLUE = 17;
constexpr int COLOR_PINK = 18;
constexpr int COLOR_ORANGE = 19;
constexpr int COLOR_RED_M = 20;
constexpr int COLOR_YELLOW_M = 21;
constexpr int COLOR_GREEN_M = 22;
constexpr int COLOR_BLUE_M = 23;

constexpr int BGT_PURPLE = 1;
constexpr int BGT_LBLUE = 2;
constexpr int BGT_PINK = 3;
constexpr int BGT_ORANGE = 4;
constexpr int BGT_RED = 5;
constexpr int BGT_YELLOW = 6;
constexpr int BGT_GREEN = 7;
constexpr int BGT_BLUE = 8;
constexpr int BGT_BLACK = 9;
constexpr int BGT_WHITE = 10;

constexpr int TXT_PURPLE = 11;
constexpr int TXT_LBLUE = 12;
constexpr int TXT_PINK = 13;
constexpr int TXT_ORANGE = 14;
constexpr int TXT_RED = 15;
constexpr int TXT_YELLOW = 16;
constexpr int TXT_GREEN = 17;
constexpr int TXT_BLUE = 18;
constexpr int TXT_WHITE = 19;
constexpr int NUM_TXT_COLORS = TXT_BLUE - TXT_PURPLE + 1;

class Icons {
private:
  static Icons& icons();
  cchar_t houseChar;
  cchar_t hotelChar;
  cchar_t unselectedItemChar;
  cchar_t selectedItemChar;
  const wchar_t iconChars[5] = L"\uf015\uf1ad\uf10c\uf111";
  Icons();
public:
  static cchar_t* house();
  static cchar_t* hotel();
  static cchar_t* unselectedItem();
  static cchar_t* selectedItem();
};