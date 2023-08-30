#include "BoardItems.h"
#include <curses.h>

/* #region BoardItem class definition */
BoardItem::BoardItem(int index, string name, BoardItemLocation location) : index(index), name(name), location(location) {
    initWindow();
}
BoardItem::~BoardItem() {
    delwin(win);
}
void BoardItem::redraw() {
    if (win != nullptr) {
        wclear(win);
        delwin(win);
    }
    initWindow();
}
void BoardItem::initWindow() {
    if (location == Bottom) {
        win = newwin(
            V_PROPERTY_HEIGHT,
            V_PROPERTY_WIDTH,
            V_PROPERTY_HEIGHT + H_PROPERTY_HEIGHT * PROPERTIES_PER_SIDE, // the y value is the height of the top property row plus the total height of the 9 properties on the side
            H_PROPERTY_WIDTH + (PROPERTIES_PER_SIDE - index - 1) * V_PROPERTY_WIDTH // index 0 is the right side on the bottom row (start at mediterranean)
        );
    }
    else if (location == Top) {
        win = newwin(
            V_PROPERTY_HEIGHT,
            V_PROPERTY_WIDTH,
            0,
            H_PROPERTY_WIDTH + index * V_PROPERTY_WIDTH
        );
    }
    else if (location == Left) {
        win = newwin(
            H_PROPERTY_HEIGHT,
            H_PROPERTY_WIDTH,
            V_PROPERTY_HEIGHT + (PROPERTIES_PER_SIDE - index - 1) * H_PROPERTY_HEIGHT, // index 0 is at the bottom at st. charles
            0
        );
    }
    else if (location == Right) {
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
    }
    else { // Left or Right
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
    case Right:
        // The vertical labels are 2 high, which means horizontal ones are about 4 wide, plus the border
        wmove(win, 1, 0);
        break;
    }

    wprintw(win, displayName.c_str());

    wattroff(win, COLOR_PAIR(colorGroup));

    if (location <= Top) {
        mvwvline(win, 1, 0, 0, V_PROPERTY_HEIGHT - 2);
        // right above the border if it's bottom, and right next to the left border
        wmove(win, location == Bottom ? V_PROPERTY_HEIGHT - 2 : 1, 1); // get ready to print the price string
    }
    else {
        mvwvline(win, 1, 0, 0, H_PROPERTY_HEIGHT - 2);
        // we only do - 1 here because the horizontal properties don't do the bottom border (on the other hand, the vertical properties don't do the right border)
        wmove(win, H_PROPERTY_HEIGHT - 1, 3);
    }

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
void RandomDraw::drawInitial() {
    if (location <= Top) { // Top or Bottom
        mvwprintw(win, V_PROPERTY_HEIGHT / 2 - 1, 0, type == Chance ? CHANCE_DISPLAY_V : COMMUNITY_CHEST_DISPLAY_V);
        // bottom tee and top tee - instead of being corners, they are Ts - connected to other property cards
        wborder(win, 0, ' ', 0, 0, ACS_TTEE, ACS_HLINE, ACS_BTEE, ACS_HLINE);
    }
    else { // Left or Right
        mvwprintw(win, H_PROPERTY_HEIGHT / 2, 0, type == Chance ? CHANCE_DISPLAY_H : COMMUNITY_CHEST_DISPLAY_H);
        wborder(win, 0, 0, 0, ' ', ACS_LTEE, ACS_RTEE, ACS_VLINE, ACS_VLINE);
    }

    wnoutrefresh(win);
}
/* #endregion */

/* #region TaxItem class definition */
TaxItem::TaxItem(int index, TaxType type, BoardItemLocation location) : BoardItem(index, type == Income ? "Income Tax" : "Luxury Tax", location), type(type) {}
/// Draw the entire card. This should only be called at the start
void TaxItem::drawInitial() {
    if (type == Income) {
        // this is on the bottom row
        mvwprintw(win, V_PROPERTY_HEIGHT / 2 - 1, 0, INCOME_TAX_DISPLAY_V);
        mvwprintw(win, V_PROPERTY_HEIGHT - 2, 2, "PAY $");
        wattron(win, A_UNDERLINE | A_BOLD);
        wprintw(win, "200");
        wattroff(win, A_UNDERLINE | A_BOLD);
        // bottom tee and top tee - instead of being corners, they are Ts - connected to other property cards
        wborder(win, 0, ' ', 0, 0, ACS_TTEE, ACS_HLINE, ACS_BTEE, ACS_HLINE);
    }
    else {
        // luxury tax - this is on the right side
        mvwprintw(win, H_PROPERTY_HEIGHT / 2, 0, LUXURY_TAX_DISPLAY_H);
        wborder(win, 0, 0, 0, ' ', ACS_LTEE, ACS_RTEE, ACS_VLINE, ACS_VLINE);
        mvwprintw(win, H_PROPERTY_HEIGHT - 1, 4, "PAY $"); // this comes after because the border will overwrite it otherwise
        wattron(win, A_UNDERLINE | A_BOLD);
        wprintw(win, "75");
        wattroff(win, A_UNDERLINE | A_BOLD);
    }

    wnoutrefresh(win);
}
/* #endregion */