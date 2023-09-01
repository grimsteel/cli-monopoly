// CmdLineMonopoly.cpp : A command line interface to Monopoly
// Copyright (c) 2023 Siddhant Kameswar
// MIT Licensed
//

#include <iostream>
#include <string>
#include <vector>
#define PDC_WIDE
#include <curses.h>
#include "CmdLineMonopoly.h"
#include "BoardItems.h"

using namespace std;

int main()
{
  initscr();
  curs_set(FALSE);
  noecho();
  init_colors();
  
  // The first strings strings here are what show up in prompts ("Would you like to buy Baltic Avenue?")
  // The second strings here are what show up on the rendered game board. Because of spacing issues, some of them use abbreviations.
  Property properties[] = {
    Property(0, "Mediterranean Avenue",   " Mediterran ean Avenue", 60,  BGT_PURPLE,  BoardItem::Bottom),
    Property(2, "Baltic Avenue",          "   Baltic     Avenue  ", 60,  BGT_PURPLE,  BoardItem::Bottom),
    Property(4, "Reading Railroad",       "   Reading   Railroad ", 200, BGT_BLACK,   BoardItem::Bottom),
    Property(5, "Oriental Avenue",        "  Oriental    Avenue  ", 100, BGT_LBLUE,   BoardItem::Bottom),
    Property(7, "Vermont Avenue",         "  Vermont     Avenue  ", 100, BGT_LBLUE,   BoardItem::Bottom),
    Property(8, "Connecticut Avenue",     " Connecticu  t Avenue ", 120, BGT_LBLUE,   BoardItem::Bottom),

    Property(0, "St. Charles Place",      " St Charles Pl.",        140, BGT_PINK,    BoardItem::Left),
    Property(1, "Electric Company",       "  Electric Co. ",        150, BGT_BLACK,   BoardItem::Left),
    Property(2, "States Avenue",          " States Avenue ",        140, BGT_PINK,    BoardItem::Left),
    Property(3, "Virginia Avenue",        " Virginia Ave. ",        150, BGT_PINK,    BoardItem::Left),
    Property(4, "Pennsylvania Railroad",  " Penn. Railroad",        200, BGT_BLACK,   BoardItem::Left),
    Property(5, "St. James Place",        " St. James Pl. ",        180, BGT_ORANGE,  BoardItem::Left),
    Property(7, "Tennessee Avenue",       " Tennessee Ave.",        180, BGT_ORANGE,  BoardItem::Left),
    Property(8, "New York Avenue",        " New York Ave. ",        200, BGT_ORANGE,  BoardItem::Left),

    Property(0, "Kentucky Avenue",        "  Kentucky    Avenue  ", 220, BGT_RED,     BoardItem::Top),
    Property(2, "Indiana Avenue",         "  Indiana     Avenue  ", 220, BGT_RED,     BoardItem::Top),
    Property(3, "Illinois Avenue",        "  Illinois    Avenue  ", 240, BGT_RED,     BoardItem::Top),
    Property(4, "B. & O. Railroad",       "   B. & O.   Railroad ", 200, BGT_BLACK,   BoardItem::Top),
    Property(5, "Atlantic Avenue",        "  Atlantic    Avenue  ", 260, BGT_YELLOW,  BoardItem::Top),
    Property(6, "Ventnor Avenue",         "  Ventnor     Avenue  ", 260, BGT_YELLOW,  BoardItem::Top),
    Property(7, "Water Works",            "   Water      Works   ", 150, BGT_BLACK,   BoardItem::Top),
    Property(8, "Marvin Gardens",         "   Marvin    Gardens  ", 280, BGT_YELLOW,  BoardItem::Top),

    Property(0, "Pacific Avenue",         "  Pacific Ave. ",        300, BGT_GREEN,   BoardItem::Right),
    Property(1, "North Carolina Avenue",  " N Carolina Ave",        300, BGT_GREEN,   BoardItem::Right),
    Property(3, "Pennsylvania Avenue",    "  Penn. Avenue ",        320, BGT_GREEN,   BoardItem::Right),
    Property(4, "Short Line",             "   Short Line  ",        200, BGT_BLACK,   BoardItem::Right),
    Property(6, "Park Place",             "   Park Place  ",        350, BGT_BLUE,    BoardItem::Right),
    Property(8, "Boardwalk",              "   Boardwalk   ",        400, BGT_BLUE,    BoardItem::Right)
  };

  RandomDraw randomDrawItems[] = {
    RandomDraw(1, RandomDraw::CommunityChest, BoardItem::Bottom),
    RandomDraw(6, RandomDraw::Chance, BoardItem::Bottom),
    RandomDraw(6, RandomDraw::CommunityChest, BoardItem::Left),
    RandomDraw(1, RandomDraw::Chance, BoardItem::Top),
    RandomDraw(2, RandomDraw::CommunityChest, BoardItem::Right),
    RandomDraw(5, RandomDraw::Chance, BoardItem::Right)
  };

  TaxItem incomeTax(3, TaxItem::Income, BoardItem::Bottom);
  TaxItem luxuryTax(7, TaxItem::Luxury, BoardItem::Right);
  Go go;
  Jail jail;
  FreeParking freeParking;
  GoToJail goToJail;

  wnoutrefresh(stdscr);

  // I wish I could use a for loop here, but that would really just create unnecessary complexity and slow the initialization down.
  // Manually ordering each of the items is by far the fastest solution
  BoardItem* boardItems[] = {
    &go,
    &properties[0],
    &randomDrawItems[0],
    &properties[1],
    &incomeTax,
    &properties[2],
    &properties[3],
    &randomDrawItems[1],
    &properties[4],
    &properties[5],
    &jail,
    &properties[6],
    &properties[7],
    &properties[8],
    &properties[9],
    &properties[10],
    &properties[11],
    &randomDrawItems[2],
    &properties[12],
    &properties[13],
    &freeParking,
    &properties[14],
    &randomDrawItems[3],
    &properties[15],
    &properties[16],
    &properties[17],
    &properties[18],
    &properties[19],
    &properties[20],
    &properties[21],
    &goToJail,
    &properties[22],
    &properties[23],
    &randomDrawItems[4],
    &properties[24],
    &properties[25],
    &randomDrawItems[5],
    &properties[26],
    &luxuryTax,
    &properties[27]
  };
  
  for (int i = 0; i < (sizeof(boardItems) / sizeof(BoardItem*)); i++) {
    boardItems[i]->drawInitial();
  }
  freeParking.fixBorder();
  goToJail.fixBorder();

  doupdate();

  keypad(stdscr, TRUE);

  while (true) {
    refresh();
    int ch = getch();

    if (ch == KEY_RESIZE) {
      resize_term(0, 0);
      wnoutrefresh(stdscr);
      // TODO: optimize this
      for (int i = 0; i < (sizeof(boardItems) / sizeof(BoardItem*)); i++) {
        boardItems[i]->redraw();
        boardItems[i]->drawInitial();
      }
      doupdate();
    } else if (ch == 'q') {
      break;
    }
  }
  
  endwin();
  return 0;
}

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
}