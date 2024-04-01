// CmdLineMonopoly.cpp : A command line interface to Monopoly
// Copyright (c) 2023-2024 Siddhant Kameswar
// MIT Licensed
//
// This program uses ncurses, which is licensed as free software under the X11/MIT License
// https://invisible-island.net/ncurses/
// License: https://invisible-island.net/ncurses/ncurses-license.html

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
