#include <string>
#include <curses.h>
#pragma once

using namespace std;

class PlayerStats
{
public:
  string name;
  PlayerStats();
  ~PlayerStats();
  void getAttributes();
private:
  WINDOW* win;
};

