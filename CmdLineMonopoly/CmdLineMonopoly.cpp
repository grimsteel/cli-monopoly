// CmdLineMonopoly.cpp : A command line interface to Monopoly
// Copyright (c) 2023 Siddhant Kameswar
// MIT Licensed
//

#define PDC_WIDE
#include <curses.h>
#include "BoardItems.h"
#include "BoardState.h"
#include "EmulatedBoardState.h"
#include "Player.h"
#include "colors.h"
#include <locale.h>

int main(int argc, char* argv[])
{
  initscr();
  curs_set(FALSE);
  noecho();
  init_colors();
  keypad(stdscr, TRUE);

  if (argc == 2) {
    EmulatedBoardState boardState(argv[1]);
    boardState.run();
  } else {
    BoardState boardState;
    boardState.run();
  }

  endwin();

  return 0;
}
