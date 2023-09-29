#include "BoardCenter.h"
#include <curses.h>

BoardCenter::BoardCenter() {
  win = newwin(10, 10, 50, 50);
}

BoardCenter::showDiceRoll(unsigned char roll1, unsigned char roll2) {

}