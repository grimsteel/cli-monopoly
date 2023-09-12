#pragma once

#include <curses.h>
#include <string>
#include <random>

using namespace std;

class BoardState
{
public:
    BoardState();
    ~BoardState();
    void setPlayerName(string name);
    void setLocation(string location);
    bool setYesNoPrompt(string prompt);
    unsigned char rollDice();
private:
    WINDOW* win;
    random_device rd;
    mt19937 mt;
    uniform_int_distribution<unsigned short> dice;
};