﻿#include "BoardItems.h"
#include "BoardState.h"
#define PDC_WIDE
#include <curses.h>
#include <cstdio>
#include "colors.h"

#pragma region BoardItem class definition
BoardItem::BoardItem(unsigned char index, string name, BoardItemLocation location) : index(index), name(name), location(location) {
  initWindow();
}
BoardItem::~BoardItem() {
  if (win != nullptr) {
    delwin(win);
  }
}
void BoardItem::redraw() {
  if (win != nullptr) {
    wclear(win);
    delwin(win);
  }
  initWindow();
}
void BoardItem::initWindow() {
  // The four sides
  if (location & Bottom) {
    playerListY = 5;
    if (location & Left) {
      win = C_WIN(BOTTOM_OFFSET, 0);
    } else if (location & Right) {
      win = C_WIN(BOTTOM_OFFSET, RIGHT_OFFSET);
    } else {
      win = V_WIN(
        BOTTOM_OFFSET,
        // index 0 is the right side on the bottom row (start at mediterranean)
        H_PROPERTY_WIDTH + (PROPERTIES_PER_SIDE - index - 1) * V_PROPERTY_WIDTH - 1 // subtract one because the bottom row needs to start on the same column as the left
      );
    }
  } else if (location & Top) {
    playerListY = 2;
    if (location & Left) {
      win = C_WIN(0, 0);
    } else if (location & Right) {
      win = C_WIN(0, RIGHT_OFFSET);
    } else {
      win = V_WIN(
        0,
        H_PROPERTY_WIDTH + index * V_PROPERTY_WIDTH - 1
      );
    }
  } else if (location & Left) {
    playerListY = 3;
    win = H_WIN(
      V_PROPERTY_HEIGHT + (PROPERTIES_PER_SIDE - index - 1) * H_PROPERTY_HEIGHT - 1, // index 0 is at the bottom at st. charles
      0
    );
  } else if (location & Right) {
    playerListY = 3;
    win = H_WIN(
      V_PROPERTY_HEIGHT + index * H_PROPERTY_HEIGHT - 1,
      RIGHT_OFFSET
    );
  }
}
void BoardItem::handlePlayer(Player* player, BoardState* boardState, RollInfo* info) {
  cchar_t playerChar;
  setcchar(&playerChar, L"\uf4ff", 0, player->color, NULL);
  mvwadd_wch(win, playerListY, player->id + 1, &playerChar);

  wnoutrefresh(win);
}
void BoardItem::handlePlayerLeave(unsigned char playerId) {
  mvwaddch(win, playerListY, playerId + 1, ' ');

  wnoutrefresh(win);
}
#pragma endregion

#pragma region Property class definition
Property::Property(unsigned char index, string name, string displayName, Prices prices, unsigned char colorGroup, BoardItemLocation location)
  : BoardItem(index, name, location), displayName(displayName), prices(prices), colorGroup(colorGroup) {}

/// Draw the entire card. This should only be called at the start
void Property::drawInitial() {
  if (location <= Top) { // Top or Bottom
    // bottom tee and top tee - instead of being corners, they are Ts - connected to other property cards
    wborder(win, 0, ' ', 0, 0, ACS_TTEE, ACS_HLINE, ACS_BTEE, ACS_HLINE);
  } else { // Left or Right
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

  waddstr(win, displayName.c_str());

  wattroff(win, COLOR_PAIR(colorGroup));

  if (location <= Top) {
    mvwvline(win, 1, 0, 0, V_PROPERTY_HEIGHT - 2);
    // right above the border if it's bottom, and right next to the left border
    wmove(win, location & Bottom ? V_PROPERTY_HEIGHT - 2 : 1, 1); // get ready to print the price string
  } else {
    mvwvline(win, 1, 0, 0, H_PROPERTY_HEIGHT - 2);
    // we only do - 1 here because the horizontal properties don't do the bottom border (on the other hand, the vertical properties don't do the right border)
    wmove(win, H_PROPERTY_HEIGHT - 1, 3);
  }

  waddstr(win, "PRICE $");
  wattron(win, A_UNDERLINE | A_BOLD);
  waddstr(win, to_string(prices.price).c_str());
  wattroff(win, A_UNDERLINE | A_BOLD);

  wnoutrefresh(win);
}
void Property::handlePlayer(Player* player, BoardState* boardState, RollInfo* info) {
  BoardItem::handlePlayer(player, boardState, info);

  // If they spawned in here, don't prompt for purchase or do rent
  if (info->roll == 0) return;
  
  if (ownedBy == 255) {
    bool shouldBuy = player->getBalance() >= prices.price && boardState->setYesNoPrompt("Would you like to buy this property?");
    if (shouldBuy) {
      char boughtCString[100];
      snprintf(boughtCString, 100, "Bought %s", name.c_str());
      player->alterBalance(-prices.price, string(boughtCString));
      ownedBy = player->id;
      player->addProperty(this);
      drawPlayerOwn(player);
    }
  } else if (numHouses != 255) { // only if it's not mortgaged
    char rentCString[100];
    snprintf(rentCString, 100, "Rent for %s", name.c_str());
    string rentString(rentCString);

    bool isUtility = player->boardItemIndex == 12 || player->boardItemIndex == 28;
    bool isRailroad = player->boardItemIndex % 10 == 5;
    
    short rent = prices.rent[numHouses];

    if (isRailroad) {
      unsigned char numRailroads = boardState->numRailroadsOwned(ownedBy);
      static constexpr int railroadMultiplier = 25;
      rent = railroadMultiplier * (1 << (numRailroads - 1));

      if (info->isChanceMultiplied) rent *= 2;
    } else if (isUtility) {
      // Account for "Advance to nearest utility and pay 10x rent"
      short multiplier = (info->isChanceMultiplied || boardState->ownsBothUtilities(ownedBy)) ? 10 : 4;
      rent = multiplier * info->roll;
    }

    player->alterBalance(-rent, rentString);
    boardState->players[ownedBy].alterBalance(rent, rentString);
  }
}

void Property::drawPlayerOwn(Player* player) {
  cchar_t playerOwnChar;
  setcchar(&playerOwnChar, L"\uf4ca", 0, player->color, NULL);

  // Clear the price line and print the player char
  switch (location) {
  case Top:
    wmove(win, playerListY - 1, 1);
    wadd_wch(win, &playerOwnChar);
    waddstr(win, "         ");
    break;
  case Left:
    wmove(win, playerListY + 1, 1);
    wadd_wch(win, &playerOwnChar);
    waddstr(win, "             ");
    break;
  case Right:
    wmove(win, playerListY + 1, 1);
    wadd_wch(win, &playerOwnChar);
    waddstr(win, "             ");
    break;
  case Bottom:
    wmove(win, playerListY + 1, 1);
    wadd_wch(win, &playerOwnChar);
    waddstr(win, "         ");
    break;
  }

  wnoutrefresh(win);
}

void Property::setHouses(unsigned char newHouses) {
  numHouses = newHouses;
  wmove(win, playerListY + 1, 3);
  wclrtoeol(win);
  if (numHouses >= 1 && numHouses <= 4) {
    whline_set(win, Icons::house(), numHouses);
  }
  else if (numHouses == 5) {
    wadd_wch(win, Icons::hotel());
  }
  else if (numHouses == 255) {
    wattron(win, COLOR_PAIR(TXT_YELLOW));
    waddstr(win, "Mort.");
    wattroff(win, COLOR_PAIR(TXT_YELLOW));
  }
  
  wnoutrefresh(win);
}
#pragma endregion

#pragma region RandomDraw class definition
RandomDraw::RandomDraw(unsigned char index, RandomDrawType type, BoardItemLocation location) : BoardItem(index, type == Chance ? "Chance" : "Community Chest", location), type(type) {}
/// Draw the entire card. This should only be called at the start
void RandomDraw::drawInitial() {
  if (location <= Top) { // Top or Bottom
    mvwaddstr(win, V_PROPERTY_HEIGHT / 2 - 1, 0, type == Chance ? CHANCE_DISPLAY_V : COMMUNITY_CHEST_DISPLAY_V);
    // bottom tee and top tee - instead of being corners, they are Ts - connected to other property cards
    wborder(win, 0, ' ', 0, 0, ACS_TTEE, ACS_HLINE, ACS_BTEE, ACS_HLINE);
  } else { // Left or Right
    mvwaddstr(win, H_PROPERTY_HEIGHT / 2, 0, type == Chance ? CHANCE_DISPLAY_H : COMMUNITY_CHEST_DISPLAY_H);
    wborder(win, 0, 0, 0, ' ', ACS_LTEE, ACS_RTEE, ACS_VLINE, ACS_VLINE);
  }

  wnoutrefresh(win);
}

void RandomDraw::handlePlayer(Player* player, BoardState* boardState, RollInfo* info) {
    BoardItem::handlePlayer(player, boardState, info);

    boardState->showChanceDraw(player->id, type, info);
}
#pragma endregion

#pragma region TaxItem class definition
TaxItem::TaxItem(unsigned char index, TaxType type, BoardItemLocation location) : BoardItem(index, type == Income ? "Income Tax" : "Luxury Tax", location), type(type) {}
/// Draw the entire card. This should only be called at the start
void TaxItem::drawInitial() {
  if (type == Income) {
    // this is on the bottom row
    mvwaddstr(win, V_PROPERTY_HEIGHT / 2 - 1, 0, INCOME_TAX_DISPLAY_V);
    mvwaddstr(win, V_PROPERTY_HEIGHT - 2, 2, "PAY $");
    wattron(win, A_UNDERLINE |  A_BOLD);
    waddstr(win, "200");
    wattroff(win, A_UNDERLINE | A_BOLD);
    // bottom tee and top tee - instead of being corners, they are Ts - connected to other property cards
    wborder(win, 0, ' ', 0, 0, ACS_TTEE, ACS_HLINE, ACS_BTEE, ACS_HLINE);
  } else {
    // luxury tax - this is on the right side
    mvwaddstr(win, H_PROPERTY_HEIGHT / 2, 0, LUXURY_TAX_DISPLAY_H);
    wborder(win, 0, 0, 0, ' ', ACS_LTEE, ACS_RTEE, ACS_VLINE, ACS_VLINE);
    mvwaddstr(win, H_PROPERTY_HEIGHT - 1, 4, "PAY $"); // this comes after because the border will overwrite it otherwise
    wattron(win, A_UNDERLINE | A_BOLD);
    waddstr(win, "75");
    wattroff(win, A_UNDERLINE | A_BOLD);
  }

  wnoutrefresh(win);
}
void TaxItem::handlePlayer(Player *player, BoardState *mainMenu, RollInfo* info) {
  BoardItem::handlePlayer(player, mainMenu, info);

  if (type == Income) {
    short totalValue = player->getTotalValue();
    short incomeTax = totalValue < 200 ? totalValue / 10 : 200;
    player->alterBalance(-incomeTax, "Income Tax");
  } else {
    player->alterBalance(-75, "Luxury Tax");
  }
}
#pragma endregion

#pragma region Corners
Go::Go() : BoardItem(0, "Go", Bottom | Right) {}
void Go::drawInitial() {
  mvwaddwstr(win, 1, 0, L"    ▄▄▄  ▄▄▄\n    █    █ █\n    █ █  █ █\n    ▀▀▀  ▀▀▀");
  mvwaddstr(win, V_PROPERTY_HEIGHT - 2, 2, "Collect $");
  wattron(win, A_UNDERLINE | A_BOLD);
  waddstr(win, "200");
  wattroff(win, A_UNDERLINE | A_BOLD);
  wborder(win, 0, 0, 0, 0, ACS_PLUS, ACS_RTEE, ACS_BTEE, 0);
  //mvwaddch(win, 2, 2, ACS_BLOCK);
  //mvwaddwstr(win, V_PROPERTY_HEIGHT / 2 - 1, H_PROPERTY_WIDTH / 2 - 1, L"Go");
  wnoutrefresh(win);
}

Jail::Jail() : BoardItem(0, "Jail (Just Visiting)", Bottom | Left) {}
void Jail::drawInitial() {
  wborder(win, 0, 0, 0, 0, ACS_LTEE, ACS_PLUS, 0, ACS_BTEE);
  mvwaddch(win, 0, JAIL_X, ACS_TTEE); // the -2 comes from the border and the fact that we need to get in front of the char
  mvwvline(win, 1, JAIL_X, 0, JAIL_HEIGHT);
  mvwaddch(win, JAIL_HEIGHT + 1, JAIL_X, ACS_LLCORNER);
  whline(win, 0, JAIL_WIDTH);
  mvwaddch(win, JAIL_HEIGHT + 1, H_PROPERTY_WIDTH - 1, ACS_RTEE);
  mvwaddstr(win, V_PROPERTY_HEIGHT - 2, 2, "Just Visiting");
  mvwaddstr(win, 2, JAIL_X + 2, "Jail");
  wnoutrefresh(win);
}
void Jail::arrestPlayer(Player* player, BoardState* boardState) {
  cchar_t playerChar;
  player->turnsInJail = 0;
  setcchar(&playerChar, L"\uf4ff", 0, player->color, NULL);
  mvwadd_wch(win, 1, 9 + player->id, &playerChar);

  wnoutrefresh(win);
}
void Jail::releasePlayer(unsigned char playerId) {
  mvwaddch(win, 1, 9 + playerId, ' ');
  wnoutrefresh(win);
}

FreeParking::FreeParking() : BoardItem(0, "Free Parking", Top | Left) {}
void FreeParking::drawInitial() {
  wborder(win, 0, 0, 0, 0, 0, ACS_TTEE, ACS_LTEE, 0);
  mvwaddstr(win, V_PROPERTY_HEIGHT / 2 - 1, 2, "Free Parking");
  wnoutrefresh(win);
}
void FreeParking::fixBorder() {
  mvwaddch(win, V_PROPERTY_HEIGHT - 1, H_PROPERTY_WIDTH - 1, ACS_PLUS);
  wnoutrefresh(win);
}
GoToJail::GoToJail() : BoardItem(0, "Go to Jail", Top | Right) {}
void GoToJail::drawInitial() {
  wborder(win, 0, 0, 0, 0, ACS_TTEE, 0, 0, ACS_RTEE);
  mvwaddstr(win, V_PROPERTY_HEIGHT / 2 - 1, 3, "Go to Jail");
  wnoutrefresh(win);
}
void GoToJail::fixBorder() {
  mvwaddch(win, V_PROPERTY_HEIGHT - 1, 0, ACS_PLUS);
  wnoutrefresh(win);
}
#pragma endregion
