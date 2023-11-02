﻿#define PDC_WIDE

#include "BoardState.h"
#include <curses.h>
#include <string>
#include <random>
#include "BoardItems.h"
#include "utils.h"
#include "colors.h"

using namespace std;

BoardState::BoardState() : mt(rd()), dice(1, 6),
  // The first strings here are what show up in prompts ("Would you like to buy Baltic Avenue?")
  // The second strings here are what show up on the rendered game board. Because of spacing issues, some of them use abbreviations.
  properties {
    Property(0, "Mediterranean Avenue",   " Mediterran ean Avenue", Property::Prices { 60, 50, { 2, 10, 30, 90, 160, 250 } },  BGT_PURPLE,  BoardItem::Bottom),
    Property(2, "Baltic Avenue",          "   Baltic     Avenue  ", Property::Prices { 60, 50, { 4, 20, 60, 180, 320, 450 } },  BGT_PURPLE,  BoardItem::Bottom),
    Property(4, "Reading Railroad",       "   Reading   Railroad ", Property::Prices { 200 }, BGT_BLACK,   BoardItem::Bottom),
    Property(5, "Oriental Avenue",        "  Oriental    Avenue  ", Property::Prices { 100, 50, { 6, 30, 90, 270, 400, 550 } }, BGT_LBLUE,   BoardItem::Bottom),
    Property(7, "Vermont Avenue",         "  Vermont     Avenue  ", Property::Prices { 100, 50, { 6, 30, 90, 270, 400, 550 } }, BGT_LBLUE,   BoardItem::Bottom),
    Property(8, "Connecticut Avenue",     " Connecticu  t Avenue ", Property::Prices { 120, 50, { 8, 40, 100, 300, 450, 600 } }, BGT_LBLUE,   BoardItem::Bottom),

    Property(0, "St. Charles Place",      " St Charles Pl.",        Property::Prices { 140, 100, { 10, 50, 150, 450, 625, 750 } }, BGT_PINK,    BoardItem::Left),
    Property(1, "Electric Company",       "  Electric Co. ",        Property::Prices { 150 }, BGT_BLACK,   BoardItem::Left),
    Property(2, "States Avenue",          " States Avenue ",        Property::Prices { 140, 100, { 10, 50, 150, 450, 625, 750 } }, BGT_PINK,    BoardItem::Left),
    Property(3, "Virginia Avenue",        " Virginia Ave. ",        Property::Prices { 160, 100, { 12, 60, 180, 500, 700, 900 } }, BGT_PINK,    BoardItem::Left),
    Property(4, "Pennsylvania Railroad",  " Penn. Railroad",        Property::Prices { 200 }, BGT_BLACK,   BoardItem::Left),
    Property(5, "St. James Place",        " St. James Pl. ",        Property::Prices { 180, 100, { 14, 70, 200, 550, 750, 950 } }, BGT_ORANGE,  BoardItem::Left),
    Property(7, "Tennessee Avenue",       " Tennessee Ave.",        Property::Prices { 180, 100, { 14, 70, 200, 550, 750, 950 } }, BGT_ORANGE,  BoardItem::Left),
    Property(8, "New York Avenue",        " New York Ave. ",        Property::Prices { 200, 100, { 16, 80, 220, 600, 800, 1000 } }, BGT_ORANGE,  BoardItem::Left),

    Property(0, "Kentucky Avenue",        "  Kentucky    Avenue  ", Property::Prices { 220, 150, { 18, 90, 250, 700, 875, 1050 } }, BGT_RED,     BoardItem::Top),
    Property(2, "Indiana Avenue",         "  Indiana     Avenue  ", Property::Prices { 220, 150, { 18, 90, 250, 700, 875, 1050 } }, BGT_RED,     BoardItem::Top),
    Property(3, "Illinois Avenue",        "  Illinois    Avenue  ", Property::Prices { 240, 150, { 20, 100, 300, 750, 925, 1100 } }, BGT_RED,     BoardItem::Top),
    Property(4, "B. & O. Railroad",       "   B. & O.   Railroad ", Property::Prices { 200 }, BGT_BLACK,   BoardItem::Top),
    Property(5, "Atlantic Avenue",        "  Atlantic    Avenue  ", Property::Prices { 260, 150, { 22, 110, 330, 800, 975, 1150 } }, BGT_YELLOW,  BoardItem::Top),
    Property(6, "Ventnor Avenue",         "  Ventnor     Avenue  ", Property::Prices { 260, 150, { 22, 110, 330, 800, 975, 1150 } }, BGT_YELLOW,  BoardItem::Top),
    Property(7, "Water Works",            "   Water      Works   ", Property::Prices { 150 }, BGT_BLACK,   BoardItem::Top),
    Property(8, "Marvin Gardens",         "   Marvin    Gardens  ", Property::Prices { 280, 150, { 24, 120, 360, 850, 1025, 1200 } }, BGT_YELLOW,  BoardItem::Top),

    Property(0, "Pacific Avenue",         "  Pacific Ave. ",        Property::Prices { 300, 200, { 26, 130, 390, 900, 1100, 1275 } }, BGT_GREEN,   BoardItem::Right),
    Property(1, "North Carolina Avenue",  " N Carolina Ave",        Property::Prices { 300, 200, { 26, 130, 390, 900, 1100, 1275 } }, BGT_GREEN,   BoardItem::Right),
    Property(3, "Pennsylvania Avenue",    "  Penn. Avenue ",        Property::Prices { 320, 200, { 28, 150, 450, 1000, 1200, 1400 } }, BGT_GREEN,   BoardItem::Right),
    Property(4, "Short Line",             "   Short Line  ",        Property::Prices { 200 }, BGT_BLACK,   BoardItem::Right),
    Property(6, "Park Place",             "   Park Place  ",        Property::Prices { 350, 200, { 35, 175, 500, 1100, 1300, 1500 } }, BGT_BLUE,    BoardItem::Right),
    Property(8, "Boardwalk",              "   Boardwalk   ",        Property::Prices { 400, 200, { 50, 200, 600, 1400, 1700, 2000 } }, BGT_BLUE,    BoardItem::Right)
  },
  randomDrawItems {
    RandomDraw(1, RandomDraw::CommunityChest, BoardItem::Bottom),
    RandomDraw(6, RandomDraw::Chance, BoardItem::Bottom),
    RandomDraw(6, RandomDraw::CommunityChest, BoardItem::Left),
    RandomDraw(1, RandomDraw::Chance, BoardItem::Top),
    RandomDraw(2, RandomDraw::CommunityChest, BoardItem::Right),
    RandomDraw(5, RandomDraw::Chance, BoardItem::Right)
  },
  incomeTax(3, TaxItem::Income, BoardItem::Bottom),
  luxuryTax(7, TaxItem::Luxury, BoardItem::Right),
  // I wish I could use a for loop here, but that would really just create unnecessary complexity and slow the initialization down.
  // Manually ordering each of the items is by far the fastest solution
  boardItems {
    &go,

    &properties[0],
    &randomDrawItems[0],
    &properties[1],
    &incomeTax,
    &properties[2],
    &properties[3],
    &randomDrawItems[1],
    &properties[4],
    &properties[5],
    
    &jail,
    
    &properties[6],
    &properties[7],
    &properties[8],
    &properties[9],
    &properties[10],
    &properties[11],
    &randomDrawItems[2],
    &properties[12],
    &properties[13],
    
    &freeParking,
    
    &properties[14],
    &randomDrawItems[3],
    &properties[15],
    &properties[16],
    &properties[17],
    &properties[18],
    &properties[19],
    &properties[20],
    &properties[21],
    
    &goToJail,
    
    &properties[22],
    &properties[23],
    &randomDrawItems[4],
    &properties[24],
    &properties[25],
    &randomDrawItems[5],
    &properties[26],
    &luxuryTax,
    &properties[27]
  }
{
  win = newwin(35, 75, 1, H_PROPERTY_WIDTH * 2 + V_PROPERTY_WIDTH * PROPERTIES_PER_SIDE + 2);
  keypad(win, true);
  players.reserve(MAX_PLAYERS);
  wnoutrefresh(stdscr);
}
BoardState::~BoardState() {
    delwin(win);
}
void BoardState::drawInitial() {
  for (int i = 0; i < (sizeof(boardItems) / sizeof(BoardItem*)); i++) {
    boardItems[i]->drawInitial();
  }
  freeParking.fixBorder();
  goToJail.fixBorder();
  doupdate();
}
void BoardState::getPlayers() {
  for (unsigned char i = 0; i < MAX_PLAYERS; i++) {
    players.emplace_back(i);
    auto newPlayer = players.end() - 1;
    bool shouldContinue = newPlayer->queryAttributes(this);
    // Render the player on Go
    boardItems[newPlayer->boardItemIndex]->handlePlayer(&(*newPlayer), this);
    if (!shouldContinue) break;
  }
  doupdate();
}

void BoardState::mainLoop() {
  while (true) {
    for (int i = 0; i < players.size(); i++) {
      if (doTurn(i)) {
        // Quit the game
        goto game_end;
      }
    }
  }
game_end:;
}
void BoardState::handleCharInput(int ch) {
  if (ch == KEY_RESIZE) {
    resize_term(0, 0);
    wnoutrefresh(stdscr);
    // TODO: optimize this
    for (int i = 0; i < (sizeof(boardItems) / sizeof(BoardItem*)); i++) {
      boardItems[i]->redraw();
      boardItems[i]->drawInitial();
    }
    doupdate();
  }
}

unsigned char BoardState::numRailroadsOwned(unsigned char playerId) {
  unsigned char numRailroads = 0;

  // Correspond to indices 5, 15, 25, and 35 in the boardItems array
  constexpr int railroadPropertyIndices[] = { 2, 10, 17, 25 };

  for (int i : railroadPropertyIndices) {
    if (properties[i].ownedBy == playerId) numRailroads++;
  }

  return numRailroads;
}

bool BoardState::ownsBothUtilities(unsigned char playerId) {
  // Correspond to indices 12 and 28 in the boardItems array
  return properties[7].ownedBy == playerId && properties[20].ownedBy == playerId;
}

bool BoardState::doTurn(unsigned char playerId) {
  Player* player = &players[playerId];
  unsigned char numRolls = 0;
  bool canRollAgain = true;
  BoardItem* boardItem = NULL;

  if (player->boardItemIndex == 255) {
    canRollAgain = false;
    player->turnsInJail++;
    // TODO: ways of leaving jail - doubles, pay, get out free

    drawHeader(playerId, "Jail");
  } else {
    boardItem = boardItems[player->boardItemIndex];
    drawHeader(playerId, boardItem->name);
  }
  

  while (true) {
    char result = drawMenu(canRollAgain);
    switch (result) {
      case 0: {
        // Roll dice
        unsigned char rolls = rollDice();
        numRolls++;
        unsigned char roll1 = rolls & 0b111;
        unsigned char roll2 = rolls >> 3;
        unsigned char total = roll1 + roll2;

        boardCenter.showDiceRoll(roll1, roll2);

        // If they rolled a double, they can roll again
        canRollAgain = roll1 == roll2;

        // Remove the player from the previous board item
        boardItem->handlePlayerLeave(playerId);

        // Get the new board item
        constexpr unsigned char NUM_BOARD_ITEMS = static_cast<unsigned char>(sizeof(boardItems) / sizeof(BoardItem*));
        unsigned char newBoardItemIndex = player->boardItemIndex + total;

        // If they passed Go,
        if (newBoardItemIndex >= NUM_BOARD_ITEMS) {
          // Give the salary
          player->alterBalance(200, "Salary");
          // Wrap around
          newBoardItemIndex -= NUM_BOARD_ITEMS;
        }

        // If they rolled doubles three times in a row or landed on "Go to Jail", go to jail. 
        if ((canRollAgain && numRolls == 3) || newBoardItemIndex == 30) {
          canRollAgain = false;
          newBoardItemIndex = 255; // jail
        }

        player->boardItemIndex = newBoardItemIndex;
        player->lastDiceRoll = total;
        
        if (newBoardItemIndex == 255) {
          drawHeader(playerId, "Jail");
          jail.arrestPlayer(player, this);
        }
        else {
          boardItem = boardItems[newBoardItemIndex];

          // Draw the header, then notify the board item
          drawHeader(playerId, boardItem->name);
          boardItem->handlePlayer(player, this);
        }

        doupdate();
        break;
      } case 1: {
        // Buy houses

        // Number of properties required for this color group. Only show properties in a color group where this is 0.
        unsigned char propertiesRequired[] = { 2, 3, 3, 3, 3, 3, 3, 2 };

        // Lowest number of houses in this color group. For instance, if Medit. has 3 houses but Baltic has 4, the 0th element here (c group 1) would be 3.
        // Therefore, because Medit has an equal # of houses to the value here, you can only buy
        // Baltic has a higher number, so you can only sell
        unsigned char colorGroupHouses[] = { 255, 255, 255, 255, 255, 255, 255, 255 };

        // Bitset (one bit per c group) indicating if all properties have the same # of houses, meaning the player can buy & sell on all of them
        uint8_t allHousesEqual = 0b11111111;

        // Calculate available houses in linear time
        for (auto currentProperty = player->properties.begin(); currentProperty != player->properties.end(); currentProperty++) {
          int colorGroup = (*currentProperty)->colorGroup - BGT_PURPLE;
          propertiesRequired[colorGroup]--;

          // If all three conditions are true, set the allHousesEqual bit to false
          // 1. The allHousesEqual bit must be true (don't change if it's already false)
          // 2. The colorGroupHouses item must not be uninitialized (255). If it is, then we're anyway going to set it to the current property houses soon
          // 3. The current property houses must be different than the colorGroupHouses item
          if (allHousesEqual | 1 << colorGroup && colorGroupHouses[colorGroup] != 255 && colorGroupHouses[colorGroup] != (*currentProperty)->numHouses) {
            allHousesEqual &= ~(static_cast<uint8_t>(1 << colorGroup));
          }

          // If the current property has less houses, set colorGroupHouses
          if ((*currentProperty)->numHouses < colorGroupHouses[colorGroup]) {
            colorGroupHouses[colorGroup] = (*currentProperty)->numHouses;
          }
        }

        vector<unsigned char> availableProperties;
        availableProperties.reserve(player->properties.size());

        // Iterate over properties, adding them to availableProperties
        for (unsigned char currentProperty = 0; currentProperty < player->properties.size(); currentProperty++) {
          unsigned char colorGroup = player->properties[currentProperty]->colorGroup - BGT_PURPLE;
          if (propertiesRequired[colorGroup] == 0) {
            availableProperties.push_back(currentProperty);
          }
        }

        availableProperties.shrink_to_fit();

        if (availableProperties.size() == 0) {
          break;
        }

        wmove(win, 2, 0);
        wclrtobot(win);
        wattron(win, A_UNDERLINE);
        waddstr(win, "Choose Property:\n");
        wattroff(win, A_UNDERLINE);

        unsigned char selectedProperty = 0;
        constexpr unsigned char txtColorOffset = TXT_PURPLE - BGT_PURPLE;
        cchar_t propertyDotChar;
        setcchar(&propertyDotChar, L"\uf10c", 0, 0, NULL);
        cchar_t selectedPropertyDotChar;
        setcchar(&selectedPropertyDotChar, L"\uf111", 0, player->properties[availableProperties[0]]->colorGroup + txtColorOffset, NULL);

        // Print the first item (selected) and all the other items
        wadd_wch(win, &selectedPropertyDotChar);
        wprintw(win, " %s\n", player->properties[availableProperties[0]]->name.c_str());
        for (vector<unsigned char>::iterator currentProperty = ++availableProperties.begin(); currentProperty != availableProperties.end(); currentProperty++) {
          SET_CCHAR_COLOR(propertyDotChar, player->properties[*currentProperty]->colorGroup + txtColorOffset);
          wadd_wch(win, &propertyDotChar);
          wprintw(win, " %s\n", player->properties[*currentProperty]->name.c_str());
        }

        // Have the user choose a property
        while (true) {
          wrefresh(win);

          int ch = wgetch(win);

          handleCharInput(ch);

          if ((ch == KEY_DOWN && selectedProperty < availableProperties.size() - 1) || (ch == KEY_UP && selectedProperty > 0)) {
            SET_CCHAR_COLOR(propertyDotChar, player->properties[availableProperties[selectedProperty]]->colorGroup + txtColorOffset);
            mvwadd_wch(win, 3 + selectedProperty, 0, &propertyDotChar);
            // KEY_UP is 1+KEY_DOWN
            // If they pressed down, it evaluates to 0 * 2 + 1 which is 1.
            // If they pressed up, it evaluates to -1 * 2 + 1 which is -1
            selectedProperty += (KEY_DOWN - ch) * 2 + 1;
            SET_CCHAR_COLOR(selectedPropertyDotChar, player->properties[availableProperties[selectedProperty]]->colorGroup + txtColorOffset);
            mvwadd_wch(win, 3 + selectedProperty, 0, &selectedPropertyDotChar);
          } else if (ch == KEY_ENTER || ch == '\n') {
            Property* property = player->properties[availableProperties[selectedProperty]];

            // Prompt whether they would like to buy or sell 1 house

            break;
          } else if (ch == 033) { // ESC character
            break;
          }
        }
        
        break;
      } case 3: {
        // List all properties
        
        unsigned char selectedProperty = 0;
        constexpr unsigned char txtColorOffset = TXT_PURPLE - BGT_PURPLE;
        cchar_t propertyDotChar;
        setcchar(&propertyDotChar, L"\uf10c", 0, 0, NULL);
        cchar_t selectedPropertyDotChar;
        setcchar(&selectedPropertyDotChar, L"\uf111", 0, properties[0].colorGroup + txtColorOffset, NULL);
        
        // Print the header and clear everything else
        wmove(win, 2, 0);
        wclrtobot(win);
        wattron(win, A_UNDERLINE);
        waddstr(win, "Choose Property:\n");
        wattroff(win, A_UNDERLINE);

        // Print the first item (selected) and all the other items
        wadd_wch(win, &selectedPropertyDotChar);
        wprintw(win, " %s\n", properties[0].name.c_str());
        for (int i = 1; i < sizeof(properties) / sizeof(Property); i++) {
          SET_CCHAR_COLOR(propertyDotChar, properties[i].colorGroup + txtColorOffset);
          wadd_wch(win, &propertyDotChar);
          wprintw(win, " %s\n", properties[i].name.c_str());
        }
        

        while (true) {
          wrefresh(win);

          int ch = wgetch(win);

          handleCharInput(ch);

          if ((ch == KEY_DOWN && selectedProperty < sizeof(properties) / sizeof(Property) - 1) || (ch == KEY_UP && selectedProperty > 0)) {
            SET_CCHAR_COLOR(propertyDotChar, properties[selectedProperty].colorGroup + txtColorOffset);
            mvwadd_wch(win, 3 + selectedProperty, 0, &propertyDotChar);
            // KEY_UP is 1+KEY_DOWN
            // If they pressed down, it evaluates to 0 * 2 + 1 which is 1.
            // If they pressed up, it evaluates to -1 * 2 + 1 which is -1
            selectedProperty += (KEY_DOWN - ch) * 2 + 1;
            SET_CCHAR_COLOR(selectedPropertyDotChar, properties[selectedProperty].colorGroup + txtColorOffset);
            mvwadd_wch(win, 3 + selectedProperty, 0, &selectedPropertyDotChar);
          } else if (ch == KEY_ENTER || ch == '\n') {
            Property* property = &properties[selectedProperty];

            wattron(win, A_UNDERLINE);
            mvwadd_wch(win, 2, 0, &selectedPropertyDotChar);
            wclrtobot(win);
            wprintw(win, " %s: \n", property->name.c_str());
            wattroff(win, A_UNDERLINE);

            wprintw(
              win,
              "Price: $%d\nMortgage Value: $%d\nBuilding Price: $%d\nRent:\n   $%d\n1  $%d\n2  $%d\n3  $%d\n4  $%d\n   $%d\n",
              property->prices.price, property->prices.price / 2, property->prices.buildingPrice,
              property->prices.rent[0], property->prices.rent[1], property->prices.rent[2],
              property->prices.rent[3], property->prices.rent[4], property->prices.rent[5]
            );
            if (property->numHouses == 5) waddstr(win, "Has Hotel\n");
            else if (property->numHouses == 255) waddstr(win, "Mortgaged\n");
            else if (property->numHouses > 0) wprintw(win, "%d Houses\n", property->numHouses);

            if (property->ownedBy != 255) {
              cchar_t ownerChar;
              setcchar(&ownerChar, L"\uf4ca", 0, players[property->ownedBy].color, NULL);
              wadd_wch(win, &ownerChar);
              wprintw(win, " Owned by %s", players[property->ownedBy].name.c_str());
            }

            // Print the house icons
            cchar_t houseChar;
            setcchar(&houseChar, L"\uf015", 0, 0, NULL);
            mvwvline_set(win, 8, 1, &houseChar, 4);
            // Hotel icon (Surrogate pair so capital U)
            mvwaddwstr(win, 12, 1, L"\U000f02dd");
            wrefresh(win);
            wgetch(win);
            break;
          } else if (ch == 033) { // ESC character
            break;
          }
        }
        break;
      } case 4: {
        return true;
      } case 5: {
        return false;
      }
    }
  }
}

void BoardState::drawHeader(unsigned char playerId, string location) {
  wattron(win, A_BOLD | A_UNDERLINE);
  mvwprintw(win, 0, 0, "%s, it's your turn.", players[playerId].name.c_str());
  wclrtoeol(win);
  wattroff(win, A_BOLD | A_UNDERLINE);
  mvwprintw(win, 1, 0, "You are on %s.", location.c_str());
  wclrtobot(win);
}

char BoardState::drawMenu(bool showRollDice) {
  char numMenuItems = showRollDice ? NUM_MENU_ITEMS : NUM_MENU_ITEMS - 1;

  wmove(win, 2, 0);
  wclrtobot(win);
  if (showRollDice) {
    wprintw(win, "  Roll Dice\n");
  }
  wprintw(win, "  Buy Houses/Hotels\n");
  wprintw(win, "  Mortage Properties\n");
  wprintw(win, "  View Property Info\n");
  wprintw(win, "  End Game\n");
  wprintw(win, "  End Turn");

  cchar_t unselectedItemChar;
  cchar_t selectedItemChar;
  setcchar(&unselectedItemChar, L"\uf10c", 0, 0, NULL);
  setcchar(&selectedItemChar, L"\uf111", 0, TXT_GREEN, NULL);

  mvwadd_wch(win, 2, 0, &selectedItemChar);
  for (char i = 1; i < numMenuItems; i++) {
    mvwadd_wch(win, 2 + i, 0, &unselectedItemChar);
  }

  char selectedItem = 0;

  while (true) {
    wrefresh(win);

    int ch = wgetch(win);

    handleCharInput(ch);

    if (ch == KEY_UP || ch == KEY_DOWN || (ch >= '1' && ch <= '0' + numMenuItems)) {
      // The menu starts at y index = 2
      // We want to replace the char at x index = 0
      mvwadd_wch(win, 2 + selectedItem, 0, &unselectedItemChar);

      if (ch == KEY_UP) selectedItem--;
      else if (ch == KEY_DOWN) selectedItem++; // KEY_DOWN = higher y value because high y is down
      else selectedItem = ch - '1';

      if (selectedItem >= numMenuItems) selectedItem = 0;
      else if (selectedItem < 0) selectedItem = numMenuItems - 1;

      mvwadd_wch(win, 2 + selectedItem, 0, &selectedItemChar);
    }
    else if (ch == KEY_ENTER || ch == '\n') {
      break;
    }
  }

  wrefresh(win);

  if (!showRollDice) selectedItem++;

  return selectedItem;
}
bool BoardState::setYesNoPrompt(string prompt) {
  return showYesNoPrompt(win, this, prompt, 0, 2);
}
unsigned char BoardState::rollDice() {
  auto roll1 = 0; // dice(mt);
  auto roll2 = 1; // dice(mt);

  // Each roll is from 1 to 6, which can be represented with 3 bits
  // This combines the two rolls into one byte. Roll 1 is the first 3 bytes, roll 2 is the next 3
  return static_cast<unsigned char>((roll1 & 0b111) | ((roll2 & 0b111) << 3));
}
