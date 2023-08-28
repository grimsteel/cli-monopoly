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

/* #region BoardItem class definition */
BoardItem::BoardItem(int index, string name, BoardItemLocation location) : index(index), name(name), location(location) {
  initWindow();
}
BoardItem::~BoardItem() {
  delwin(win);
}
void BoardItem::initWindow() {
  if (location == Bottom) {
    win = newwin(
      V_PROPERTY_HEIGHT,
      V_PROPERTY_WIDTH,
      V_PROPERTY_HEIGHT + H_PROPERTY_HEIGHT * PROPERTIES_PER_SIDE, // the y value is the height of the top property row plus the total height of the 9 properties on the side
      H_PROPERTY_WIDTH + (PROPERTIES_PER_SIDE - index - 1) * V_PROPERTY_WIDTH // index 0 is the right side on the bottom row (start at mediterranean)
    );
  } else if (location == Top) {
    win = newwin(
      V_PROPERTY_HEIGHT,
      V_PROPERTY_WIDTH,
      0,
      H_PROPERTY_WIDTH + index * V_PROPERTY_WIDTH
    );
  } else if (location == Left) {
    win = newwin(
      H_PROPERTY_HEIGHT,
      H_PROPERTY_WIDTH,
      V_PROPERTY_HEIGHT + (PROPERTIES_PER_SIDE - index - 1) * H_PROPERTY_HEIGHT, // index 0 is at the bottom at st. charles
      0
    );
  } else if (location == Right) {
    win = newwin(
      H_PROPERTY_HEIGHT,
      H_PROPERTY_WIDTH,
      V_PROPERTY_HEIGHT + index * H_PROPERTY_HEIGHT,
      H_PROPERTY_WIDTH + V_PROPERTY_WIDTH * PROPERTIES_PER_SIDE
    );
  }
}
/* #endregion */

/* #region Property class definition */
Property::Property(int index, string name, short price, unsigned char colorGroup, BoardItemLocation location)
  : BoardItem(index, name, location), price(price), colorGroup(colorGroup) {}

/// Draw the entire card. This should only be called at the start
void Property::drawInitial(string displayName) {
  // bottom tee and top tee - instead of being corners, they are Ts - connected to other property cards
  wborder(win, 0, ' ', 0, 0, ACS_TTEE, ACS_HLINE, ACS_BTEE, ACS_HLINE);

  wattron(win, COLOR_PAIR(colorGroup));
  if (location == Bottom) {
    mvwprintw(win, 1, 0, displayName.c_str());
  } else if (location == Top) {
    // The name label is 2 high, so we move 2 + 1 (the border) above the bottom
    mvwprintw(win, V_PROPERTY_HEIGHT - 3, 0, displayName.c_str());
  }
  wattroff(win, COLOR_PAIR(colorGroup));
  
  mvwvline(win, 1, 0, 0, V_PROPERTY_HEIGHT - 2);

  if (location == Bottom) {
    mvwprintw(win, V_PRICESTRING_Y, V_PRICESTRING_X, "PRICE $");
  } else if (location == Top) {
    mvwprintw(win, 1, V_PRICESTRING_X, "PRICE $");
  }

  wattron(win, A_UNDERLINE | A_BOLD);
  wprintw(win, to_string(price).c_str());
  wattroff(win, A_UNDERLINE | A_BOLD);
  wnoutrefresh(win);
}
/* #endregion */

/* #region RandomDraw class definition */
RandomDraw::RandomDraw(int index, RandomDrawType type, BoardItemLocation location) : BoardItem(index, type == Chance ? "Chance" : "Community Chest", location), type(type) {}
/// Draw the entire card. This should only be called at the start
void RandomDraw::drawInitial(string displayName) {
  mvwprintw(win, V_PROPERTY_HEIGHT / 2 - 1, 0, displayName.c_str());

  wborder(win, 0, ' ', 0, 0, ACS_TTEE, ACS_HLINE, ACS_BTEE, ACS_HLINE);

  wnoutrefresh(win);
}
/* #endregion */

class Player {
private:
  int cash = 0;
  unsigned char position = 0;
};

int main()
{
  initscr();
  curs_set(FALSE);
  noecho();
  scrollok(stdscr, TRUE);
  start_color();
  init_pair(BGT_PURPLE, COLOR_WHITE, COLOR_MAGENTA | 0b1000); // black on light magenta
  init_pair(BGT_LBLUE, COLOR_BLACK, COLOR_CYAN | 0b1000); // black on light cyan
  init_pair(BGT_PINK, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(BGT_ORANGE, COLOR_BLACK, COLOR_RED | 0b1000);
  init_pair(BGT_RED, COLOR_WHITE, COLOR_RED);
  init_pair(BGT_YELLOW, COLOR_BLACK, COLOR_YELLOW);
  init_pair(BGT_GREEN, COLOR_WHITE, COLOR_GREEN);
  init_pair(BGT_BLUE, COLOR_WHITE, COLOR_BLUE);
  init_pair(BGT_BLACK, COLOR_WHITE, COLOR_BLACK);

  // Note: The reason I'm not using a for loop here is because I don't want to invoke copy constructors and deal with unecessary memory (de)allocation.
  // This is much faster, and is statically included in the executable.
  Property properties[] = {
    PROPERTY_B(0, "Mediterranean Avenue", 60, BGT_PURPLE),
    // community chest
    PROPERTY_B(2, "Baltic Avenue", 60, BGT_PURPLE),
    // income tax
    PROPERTY_B(4, "Reading Railroad", 200, BGT_BLACK),
    PROPERTY_B(5, "Oriental Avenue", 100, BGT_LBLUE),
    // chance
    PROPERTY_B(7, "Vermont Avenue", 100, BGT_LBLUE),
    PROPERTY_B(8, "Connecticut Avenue", 120, BGT_LBLUE),

    PROPERTY_T(0, "Kentucky Avenue", 220, BGT_RED),
    // chance
    PROPERTY_T(2, "Indiana Avenue", 220, BGT_RED),
    PROPERTY_T(3, "Illinois Avenue", 240, BGT_RED),
    PROPERTY_T(4, "B. & O. Railroad", 200, BGT_BLACK),
    PROPERTY_T(5, "Atlantic Avenue", 260, BGT_YELLOW),
    PROPERTY_T(6, "Ventnor Avenue", 260, BGT_YELLOW),
    PROPERTY_T(7, "Water Works", 150, BGT_BLACK),
    PROPERTY_T(8, "Marvin Gardens", 280, BGT_YELLOW),
  };

  RandomDraw randomDrawItems[] = {
    RandomDraw(1, RandomDraw::CommunityChest, Bottom),
    RandomDraw(6, RandomDraw::Chance, Bottom),
    RandomDraw(1, RandomDraw::Chance, Top)
  };

  wnoutrefresh(stdscr);

  BoardItem* boardItems[] = {
    &properties[0],
    &randomDrawItems[0],
    &properties[1],
    &properties[2],
    &properties[3],
    &randomDrawItems[1],
    &properties[4],
    &properties[5],

    &properties[6],
    &randomDrawItems[2],
    &properties[7],
    &properties[8],
    &properties[9],
    &properties[10],
    &properties[11],
    &properties[12],
    &properties[13],
  };
  string boardItemReadableNames[] = {
    " Mediterran ean Avenue",
    COMMUNITY_CHEST_DISPLAY,
    "   Baltic     Avenue  ",
    "   Reading   Railroad ",
    "  Oriental    Avenue  ",
    CHANCE_DISPLAY,
    "  Vermont     Avenue  ",
    " Connecticu  t Avenue ",
   /*  "   St. Charles      Place     ",
    "    Electric       Company    ",
    "     States         Avenue    ",
    "    Virginia        Avenue    ",
    "  Pennsylvania     Railroad   ",
    "    St. James       Place     ",
    COMMUNITY_CHEST_DISPLAY,
    "   Tennessee        Avenue    ", */
    "  Kentucky    Avenue  ",
    CHANCE_DISPLAY,
    "  Indiana     Avenue  ",
    "  Illinois    Avenue  ",
    "   B. & O.   Railroad ",
    "  Atlantic    Avenue  ",
    "  Ventnor     Avenue  ",
    "   Water      Works   ",
    "   Marvin    Gardens  ",
    "  Pacific     Avenue  ",
    " N. Carolina  Avenue  ",
    "Pennsylvania  Avenue  ",
    "   Short       Line   ",
    "    Park      Place   ",
  };
  
  for (int i = 0; i < (sizeof(boardItems) / sizeof(BoardItem*)); i++) {
    boardItems[i]->drawInitial(boardItemReadableNames[i]);
  }

  doupdate();

  keypad(stdscr, TRUE);

  while (true) {
    refresh();
    int ch = getch();

    if (ch == KEY_RESIZE) {
      resize_term(0, 0);
      wnoutrefresh(stdscr);
      for (int i = 0; i < (sizeof(boardItems) / sizeof(BoardItem*)); i++) {
        boardItems[i]->redraw();
        boardItems[i]->drawInitial(boardItemReadableNames[i]);
      }
      doupdate();
    } else if (ch == 'q') {
      break;
    }
  }
  
  endwin();
  return 0;
}
