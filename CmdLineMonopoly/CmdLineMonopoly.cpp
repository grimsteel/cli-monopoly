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
#include <iostream>

int main(int argc, char* argv[])
{
  // Fix for SIGWINCH
  cout << "Press enter to begin\n";
  cin.ignore();
   
  // Initialize the terminal
  setlocale(LC_ALL, "");
  initscr();
  curs_set(FALSE); // disable cursor
  noecho(); // disable echoing stdin
  init_colors();
  keypad(stdscr, TRUE); // allows arrow keys in stdin

  if (argc == 2) {
    // If we were passed an emulation file, use that
    EmulatedBoardState boardState(argv[1]);
    boardState.run();
  } else {
    // Otherwise, run normally
    BoardState boardState;
    boardState.run();
  }

  endwin();

  return 0;
}
