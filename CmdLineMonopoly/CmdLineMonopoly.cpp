// CmdLineMonopoly.cpp : A command line interface to Monopoly
// Copyright (c) 2023 Siddhant Kameswar
// MIT Licensed
//

#include <iostream>
#include <string>
#include <vector>
#include <curses.h>
#include "CmdLineMonopoly.h"

using namespace std;

/// A single Property card on the map
class Property {
public:
  Property(int x, int y, short price, unsigned char colorGroup) : price(price), colorGroup(colorGroup) {
    win = newwin(13, 20, y, x);
  }
  ~Property() {
    delwin(win);
  }
  short price;
  short mortgagePrice;
  string name;
  short housePrice;
  unsigned char ownedBy = 255; // 0-254 are player IDs, 255 is -1
  unsigned char numHouses = 0; // 5 houses equals 1 hotel
  /// Draw the entire card. This should only be called at the start
  void drawInitial(char readableName[60]) {
    wattron(win, COLOR_PAIR(colorGroup));
    mvwprintw(win, 1, 0, readableName);
    wattroff(win, COLOR_PAIR(colorGroup));
    wborder(win, 0, 0, 0, 0, ACS_TTEE, ACS_TTEE, ACS_BTEE, ACS_BTEE); // bottom tee and top tee - instead of being corners, they are Ts - connected to other property cards
    //box(win, 0, 0);
    mvwprintw(win, 11, 5, "PRICE $");
    wattron(win, A_UNDERLINE | A_BOLD);
    mvwprintw(win, 11, 12, to_string(price).c_str());
    wattroff(win, A_UNDERLINE | A_BOLD);
    wrefresh(win);
  }
private:
  WINDOW *win;
  unsigned char colorGroup;
  //char* mNameDisp[3]; // three rows of 18 characters (each property is 20 wide minus the border on each side) plus a null terminator
};

class Player {
private:
  int cash = 0;
  unsigned char position = 0;
};

int main()
{
  initscr();
  start_color();
  init_pair(BGT_PURPLE, COLOR_WHITE, COLOR_MAGENTA | 0b1000); // black on light magenta
  init_pair(BGT_LBLUE, COLOR_BLACK, COLOR_CYAN | 0b1000); // black on light cyan
  init_pair(BGT_PINK, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(BGT_ORANGE, COLOR_BLACK, COLOR_RED | 0b1000);
  init_pair(BGT_RED, COLOR_WHITE, COLOR_RED);
  init_pair(BGT_YELLOW, COLOR_BLACK, COLOR_YELLOW);
  init_pair(BGT_GREEN, COLOR_WHITE, COLOR_GREEN);
  init_pair(BGT_BLUE, COLOR_WHITE, COLOR_BLUE);

  // Note: The reason I'm not using a for loop here is because I don't want to invoke copy constructors and deal with unecessary memory (de)allocation.
  // This is much faster, and is statically included in the executable.
  Property properties[] = {
    PROPERTY(4, "Mediterranean Avenue", 60, BGT_PURPLE),
    PROPERTY(3, "Baltic Avenue", 60, BGT_PURPLE),
    PROPERTY(2, "Oriental Avenue", 100, BGT_LBLUE),
    PROPERTY(1, "Vermont Avenue", 100, BGT_LBLUE),
    PROPERTY(0, "Connecticut Avenue", 120, BGT_LBLUE)
  };

  // What does 60 mean??? -> Each property card is 20 wide. We print from x = 0 (we can't do it from x = 1 because carriage returns anyway bring it back to 0)
  // to x = 18, so 19 * 3
  // 19 * 3 + two carriage returns = 58 and the null terminator
  char readableNames[5][60] = {
    "   Mediterreanean  \n       Avenue      \n                   ",
    "                   \n    Baltic Avenue  \n                   ",
    "                   \n   Oriental Avenue \n                   ",
    "                   \n   Vermont Avenue  \n                   ",
    "                   \n Connecticut Avenue\n                   ",
  };
  
  refresh();
  for (int i = 0; i < 5; i++) {
    properties[i].drawInitial(readableNames[i]);
  }
  
  getch();
  endwin();
  return 0;
}
