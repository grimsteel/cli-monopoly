// CmdLineMonopoly.cpp : A command line interface to Monopoly
// Copyright (c) 2023 Siddhant Kameswar
// MIT Licensed
//

#include <iostream>
#include <string>
#define PDC_WIDE
#include <curses.h>
#include "CmdLineMonopoly.h"
#include "BoardItems.h"
#include "BoardState.h"
#include "Player.h"
#include "colors.h"
#include <locale.h>

using namespace std;

void init_colors();

int main()
{
  initscr();
  curs_set(FALSE);
  noecho();
  init_colors();
  keypad(stdscr, TRUE);
  
  BoardState boardState;
  boardState.drawInitial();
  boardState.getPlayers();

  while (true) {
    refresh();
    int ch = getch();

    boardState.handleCharInput(ch);

    if (ch == 'q') {
      break;
    }
  }

  endwin();

  return 0;
}
