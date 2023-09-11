#include "MainMenu.h"
#include <curses.h>
#include <string>
#include "BoardItems.h"

using namespace std;

MainMenu::MainMenu() {
    win = newwin(10, 30, 5, H_PROPERTY_WIDTH * 2 + V_PROPERTY_WIDTH + PROPERTIES_PER_SIDE + 10);
}
MainMenu::~MainMenu() {
    delwin(win);
}
void MainMenu::setLocation(string name) {

}
void MainMenu::setPlayerName(string name) {

}