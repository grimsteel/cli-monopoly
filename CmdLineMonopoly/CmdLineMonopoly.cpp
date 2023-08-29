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
  if (location <= Top) { // Top or Bottom
    // bottom tee and top tee - instead of being corners, they are Ts - connected to other property cards
    wborder(win, 0, ' ', 0, 0, ACS_TTEE, ACS_HLINE, ACS_BTEE, ACS_HLINE);
  } else { // Left or Right
    wborder(win, 0, 0, 0, ' ', ACS_LTEE, ACS_RTEE, ACS_VLINE, ACS_VLINE);
  }

  wattron(win, COLOR_PAIR(colorGroup));

  switch (location) {
    case Bottom:
      wmove(win, 1, 0);
      break;
    case Top:
      // The name label is 2 high, so we move 2 + 1 (the border) above the bottom
      wmove(win, V_PROPERTY_HEIGHT - 3, 0);
      break;
    case Left:
      // The vertical labels are 2 high, which means horizontal ones are about 4 wide, plus the border
      wmove(win, 1, H_PROPERTY_WIDTH - 5);
      break;
  }

  wprintw(win, displayName.c_str());

  wattroff(win, COLOR_PAIR(colorGroup));
  
  mvwvline(win, 1, 0, 0, V_PROPERTY_HEIGHT - 2);

  wmove(win, location == Bottom ? V_PRICESTRING_Y : 1, V_PRICESTRING_X);

  wprintw(win, "PRICE $");

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
  if (location <= Top) { // Top or Bottom
    mvwprintw(win, V_PROPERTY_HEIGHT / 2 - 1, 0, displayName.c_str());
    // bottom tee and top tee - instead of being corners, they are Ts - connected to other property cards
    wborder(win, 0, ' ', 0, 0, ACS_TTEE, ACS_HLINE, ACS_BTEE, ACS_HLINE);
  } else { // Left or Right
    mvwprintw(win, H_PROPERTY_HEIGHT / 2 - 1, 0, displayName.c_str());
    wborder(win, 0, 0, 0, ' ', ACS_LTEE, ACS_RTEE, ACS_VLINE, ACS_VLINE);
  }

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

  // Colors
  start_color();
  init_pair(BGT_PURPLE, COLOR_WHITE, COLOR_MAGENTA | 0b1000); // black on light magenta (somehow this looks purple)
  init_pair(BGT_LBLUE, COLOR_BLACK, COLOR_CYAN | 0b1000); // black on light cyan
  init_pair(BGT_PINK, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(BGT_ORANGE, COLOR_BLACK, COLOR_RED | 0b1000); // black on light red (orange?)
  init_pair(BGT_RED, COLOR_WHITE, COLOR_RED);
  init_pair(BGT_YELLOW, COLOR_BLACK, COLOR_YELLOW);
  init_pair(BGT_GREEN, COLOR_WHITE, COLOR_GREEN);
  init_pair(BGT_BLUE, COLOR_WHITE, COLOR_BLUE);
  init_pair(BGT_BLACK, COLOR_WHITE, COLOR_BLACK);

  // The strings here are what show up in prompts ("Would you like to buy Baltic Avenue?")
  Property properties[] = {
    Property(0, "Mediterranean Avenue", 60, BGT_PURPLE, Bottom),
    // community chest
    Property(2, "Baltic Avenue", 60, BGT_PURPLE, Bottom),
    // income tax
    Property(4, "Reading Railroad", 200, BGT_BLACK, Bottom),
    Property(5, "Oriental Avenue", 100, BGT_LBLUE, Bottom),
    // chance
    Property(7, "Vermont Avenue", 100, BGT_LBLUE, Bottom),
    Property(8, "Connecticut Avenue", 120, BGT_LBLUE, Bottom),

    Property(0, "St. Charles Place", 140, BGT_PINK, Left),
    Property(1, "Electric Company", 150, BGT_BLACK, Left),
    Property(2, "States Avenue", 140, BGT_PINK, Left),
    Property(3, "Virginia Avenue", 150, BGT_PINK, Left),
    Property(4, "Pennsylvania Railroad", 200, BGT_BLACK, Left),
    Property(5, "St. James Place", 180, BGT_ORANGE, Left),
    // community chest
    Property(7, "Tennessee Avenue", 180, BGT_ORANGE, Left),
    Property(8, "New York Avenue", 200, BGT_ORANGE, Left),

    Property(0, "Kentucky Avenue", 220, BGT_RED, Top),
    // chance
    Property(2, "Indiana Avenue", 220, BGT_RED, Top),
    Property(3, "Illinois Avenue", 240, BGT_RED, Top),
    Property(4, "B. & O. Railroad", 200, BGT_BLACK, Top),
    Property(5, "Atlantic Avenue", 260, BGT_YELLOW, Top),
    Property(6, "Ventnor Avenue", 260, BGT_YELLOW, Top),
    Property(7, "Water Works", 150, BGT_BLACK, Top),
    Property(8, "Marvin Gardens", 280, BGT_YELLOW, Top),
  };

  RandomDraw randomDrawItems[] = {
    RandomDraw(1, RandomDraw::CommunityChest, Bottom),
    RandomDraw(6, RandomDraw::Chance, Bottom),
    RandomDraw(6, RandomDraw::CommunityChest, Left),
    RandomDraw(1, RandomDraw::Chance, Top)
  };

  wnoutrefresh(stdscr);

  // I wish I could use a for loop here, but that would really just create unnecessary complexity and slow the initialization down.
  // Manually ordering each of the items is by far the fastest solution
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
    &properties[7],
    &properties[8],
    &properties[9],
    &properties[10],
    &properties[11],
    &randomDrawItems[2],
    &properties[12],
    &properties[13],

    &properties[14],
    &randomDrawItems[3],
    &properties[15],
    &properties[16],
    &properties[17],
    &properties[18],
    &properties[19],
    &properties[20],
    &properties[21],
  };

  // The strings here are what show up on the rendered game board. Because of spacing issues, some of them use abbreviations.
  string boardItemReadableNames[] = {
    " Mediterran ean Avenue",
    COMMUNITY_CHEST_DISPLAY,
    "   Baltic     Avenue  ",
    "   Reading   Railroad ",
    "  Oriental    Avenue  ",
    CHANCE_DISPLAY,
    "  Vermont     Avenue  ",
    " Connecticu  t Avenue ",
    "St. Charles Place",
    "Electric Company",
    "States Avenue",
    "Virginia Avenue",
    "Pennsylvania R.R.",
    "St. James Place",
    COMMUNITY_CHEST_DISPLAY,
    "Tennessee Avenue",
    "New York Avenue",
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
