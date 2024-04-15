#define PDC_WIDE

#include "BoardState.h"
#include <curses.h>
#include <string>
#include <random>
#include <unordered_set>
#include "BoardItems.h"
#include "utils.h"
#include "colors.h"

using namespace std;

BoardState::BoardState() : mt(rd()), dice(1, 6), chanceDrawer(0, 15),
  // The first strings here are what show up in prompts ("Would you like to buy Baltic Avenue?")
  // The second strings here are what show up on the rendered game board. Because of spacing issues, some of them use abbreviations.
  // Note that the railroads and utilities do not have rent data, because it's calculated at runtime. Houses also cannot be built on them
  properties {
    Property(0, "Mediterranean Avenue",   " Mediterran ean Avenue", Property::Prices { 60, 50, { 2, 10, 30, 90, 160, 250 } },  BGT_PURPLE,  BoardItem::Bottom), // 0
    Property(2, "Baltic Avenue",          "   Baltic     Avenue  ", Property::Prices { 60, 50, { 4, 20, 60, 180, 320, 450 } },  BGT_PURPLE,  BoardItem::Bottom),
    Property(4, "Reading Railroad",       "   Reading   Railroad ", Property::Prices { 200 }, BGT_BLACK,   BoardItem::Bottom),
    Property(5, "Oriental Avenue",        "  Oriental    Avenue  ", Property::Prices { 100, 50, { 6, 30, 90, 270, 400, 550 } }, BGT_LBLUE,   BoardItem::Bottom),
    Property(7, "Vermont Avenue",         "  Vermont     Avenue  ", Property::Prices { 100, 50, { 6, 30, 90, 270, 400, 550 } }, BGT_LBLUE,   BoardItem::Bottom),
    Property(8, "Connecticut Avenue",     " Connecticu  t Avenue ", Property::Prices { 120, 50, { 8, 40, 100, 300, 450, 600 } }, BGT_LBLUE,   BoardItem::Bottom),

    Property(0, "St. Charles Place",      " St Charles Pl.",        Property::Prices { 140, 100, { 10, 50, 150, 450, 625, 750 } }, BGT_PINK,    BoardItem::Left), // 6
    Property(1, "Electric Company",       "  Electric Co. ",        Property::Prices { 150 }, BGT_BLACK,   BoardItem::Left),
    Property(2, "States Avenue",          " States Avenue ",        Property::Prices { 140, 100, { 10, 50, 150, 450, 625, 750 } }, BGT_PINK,    BoardItem::Left),
    Property(3, "Virginia Avenue",        " Virginia Ave. ",        Property::Prices { 160, 100, { 12, 60, 180, 500, 700, 900 } }, BGT_PINK,    BoardItem::Left),
    Property(4, "Pennsylvania Railroad",  " Penn. Railroad",        Property::Prices { 200 }, BGT_BLACK,   BoardItem::Left),
    Property(5, "St. James Place",        " St. James Pl. ",        Property::Prices { 180, 100, { 14, 70, 200, 550, 750, 950 } }, BGT_ORANGE,  BoardItem::Left),
    Property(7, "Tennessee Avenue",       " Tennessee Ave.",        Property::Prices { 180, 100, { 14, 70, 200, 550, 750, 950 } }, BGT_ORANGE,  BoardItem::Left),
    Property(8, "New York Avenue",        " New York Ave. ",        Property::Prices { 200, 100, { 16, 80, 220, 600, 800, 1000 } }, BGT_ORANGE,  BoardItem::Left),

    Property(0, "Kentucky Avenue",        "  Kentucky    Avenue  ", Property::Prices { 220, 150, { 18, 90, 250, 700, 875, 1050 } }, BGT_RED,     BoardItem::Top), // 14
    Property(2, "Indiana Avenue",         "  Indiana     Avenue  ", Property::Prices { 220, 150, { 18, 90, 250, 700, 875, 1050 } }, BGT_RED,     BoardItem::Top),
    Property(3, "Illinois Avenue",        "  Illinois    Avenue  ", Property::Prices { 240, 150, { 20, 100, 300, 750, 925, 1100 } }, BGT_RED,     BoardItem::Top),
    Property(4, "B. & O. Railroad",       "   B. & O.   Railroad ", Property::Prices { 200 }, BGT_BLACK,   BoardItem::Top),
    Property(5, "Atlantic Avenue",        "  Atlantic    Avenue  ", Property::Prices { 260, 150, { 22, 110, 330, 800, 975, 1150 } }, BGT_YELLOW,  BoardItem::Top),
    Property(6, "Ventnor Avenue",         "  Ventnor     Avenue  ", Property::Prices { 260, 150, { 22, 110, 330, 800, 975, 1150 } }, BGT_YELLOW,  BoardItem::Top),
    Property(7, "Water Works",            "   Water      Works   ", Property::Prices { 150 }, BGT_BLACK,   BoardItem::Top),
    Property(8, "Marvin Gardens",         "   Marvin    Gardens  ", Property::Prices { 280, 150, { 24, 120, 360, 850, 1025, 1200 } }, BGT_YELLOW,  BoardItem::Top),

    Property(0, "Pacific Avenue",         "  Pacific Ave. ",        Property::Prices { 300, 200, { 26, 130, 390, 900, 1100, 1275 } }, BGT_GREEN,   BoardItem::Right), // 22
    Property(1, "North Carolina Avenue",  " N Carolina Ave",        Property::Prices { 300, 200, { 26, 130, 390, 900, 1100, 1275 } }, BGT_GREEN,   BoardItem::Right),
    Property(3, "Pennsylvania Avenue",    "  Penn. Avenue ",        Property::Prices { 320, 200, { 28, 150, 450, 1000, 1200, 1400 } }, BGT_GREEN,   BoardItem::Right),
    Property(4, "Short Line",             "   Short Line  ",        Property::Prices { 200 }, BGT_BLACK,   BoardItem::Right),
    Property(6, "Park Place",             "   Park Place  ",        Property::Prices { 350, 200, { 35, 175, 500, 1100, 1300, 1500 } }, BGT_BLUE,    BoardItem::Right),
    Property(8, "Boardwalk",              "   Boardwalk   ",        Property::Prices { 400, 200, { 50, 200, 600, 1400, 1700, 2000 } }, BGT_BLUE,    BoardItem::Right) // 27
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
    &go, // 0

    &properties[0],
    &randomDrawItems[0],
    &properties[1],
    &incomeTax,
    &properties[2], // 5
    &properties[3],
    &randomDrawItems[1],
    &properties[4],
    &properties[5],
    
    &jail, // 10
    
    &properties[6],
    &properties[7],
    &properties[8],
    &properties[9],
    &properties[10], // 15
    &properties[11],
    &randomDrawItems[2],
    &properties[12],
    &properties[13],
    
    &freeParking, // 20
    
    &properties[14],
    &randomDrawItems[3],
    &properties[15],
    &properties[16],
    &properties[17], // 25
    &properties[18],
    &properties[19],
    &properties[20],
    &properties[21],
    
    &goToJail, // 30
    
    &properties[22],
    &properties[23],
    &randomDrawItems[4],
    &properties[24],
    &properties[25], // 35
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
  BoardItem::RollInfo dummyRollInfo = {
    .roll = 0,
    .isChanceMultiplied = false
  };

  for (unsigned char i = 0; i < MAX_PLAYERS; i++) {
    players.emplace_back(i);
    auto newPlayer = players.end() - 1;
    bool shouldContinue = newPlayer->queryAttributes(this);

    // Render the player on Go
    boardItems[newPlayer->boardItemIndex]->handlePlayer(&(*newPlayer), this, &dummyRollInfo);
    if (!shouldContinue) break;
  }
  doupdate();
}

void BoardState::mainLoop() {
  while (true) {
    for (int i = 0; i < players.size(); i++) {
      if (doTurn(&players[i])) {
        // Quit the game
        return;
      }
    }
  }
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

  for (int i = 0; i < sizeof(boardItems) / sizeof(BoardItem*); i++) {
    // Railroads are at the middle of every side
    bool isRailroad = i % 10 == 5;
    if (isRailroad && dynamic_cast<Property*>(boardItems[i])->ownedBy == playerId) {
      numRailroads++;
    }
  }

  return numRailroads;
}

bool BoardState::ownsBothUtilities(unsigned char playerId) {
  // Correspond to indices 12 and 28 in the boardItems array
  return properties[7].ownedBy == playerId && properties[20].ownedBy == playerId;
}

bool BoardState::doTurn(Player* player) {
  unsigned char playerId = player->id;
  unsigned char numRolls = 0;
  bool canRollAgain = true;

  if (player->boardItemIndex == 255) {
    canRollAgain = false;
    player->turnsInJail++;
    // TODO: ways of leaving jail - doubles, pay, get out free

    drawHeader(playerId, "Jail");
    bool wasReleased = promptJailChoices(playerId);
    if (wasReleased) {
      player->turnsInJail = 0;
      canRollAgain = true;
      numRolls = 1; // don't speed right after getting out of jail
    }
  } else {
    drawHeader(playerId, boardItems[player->boardItemIndex]->name);
  }
  

  while (true) {
    char result = drawMenu(canRollAgain);
    switch (result) {
      case 0: {
        // Roll dice
        unsigned char rolls = rollDice(playerId);
        numRolls++;
        unsigned char roll1 = rolls & 0b111;
        unsigned char roll2 = rolls >> 3;
        unsigned char total = roll1 + roll2;

        boardCenter.showDiceRoll(roll1, roll2);

        // If they rolled a double, they can roll again
        canRollAgain = roll1 == roll2;

        // Remove the player from the previous board item
        boardItems[player->boardItemIndex]->handlePlayerLeave(playerId);

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
        
        if (newBoardItemIndex == 255) {
          drawHeader(playerId, "Jail");
          jail.arrestPlayer(player, this);
        }
        else {
          // Draw the header, then notify the board item
          drawHeader(playerId, boardItems[player->boardItemIndex]->name);

          BoardItem::RollInfo rollInfo = {
            .roll = total,
            .isChanceMultiplied = false
          };
          boardItems[player->boardItemIndex]->handlePlayer(player, this, &rollInfo);
        }

        doupdate();
        break;
      } case 1: {
        // Buy houses

        // Number of properties required for this color group. Only show properties in a color group where this is 0.
        unsigned char propertiesRequired[] = { 2, 3, 3, 3, 3, 3, 3, 2 };

        // Bitset (one bit per c group) indicating if all properties have the same # of houses, meaning the player can buy & sell on all of them
        uint8_t allHousesEqual = 0b11111111;

        // Calculate color groups where all properties are owned
        for (auto currentProperty = player->properties.begin(); currentProperty != player->properties.end(); currentProperty++) {
          // Skip utilities, railroads, and mortgaged properties
          if ((*currentProperty)->colorGroup == BGT_BLACK || (*currentProperty)->getHouses() == 255) continue;

          int colorGroup = (*currentProperty)->colorGroup - BGT_PURPLE;
          propertiesRequired[colorGroup]--;
        }

        vector<unsigned char> availableColorGroups;
        availableColorGroups.reserve(sizeof(propertiesRequired) / sizeof(unsigned char));
        for (int i = 0; i < sizeof(propertiesRequired) / sizeof(unsigned char); i++) {
          if (propertiesRequired[i] == 0) availableColorGroups.push_back(i);
        }
        availableColorGroups.shrink_to_fit();

        if (availableColorGroups.size() == 0) break;

        vector<Property*> colorGroupProperties = promptChooseProperty(availableColorGroups, true, 255);
        if (colorGroupProperties.size() == 0) break;

        unsigned char colorGroup = colorGroupProperties[0]->colorGroup;
        cchar_t propertyDotChar;
        setcchar(&propertyDotChar, L"\uf10c", 0, colorGroup + TXT_PURPLE - BGT_PURPLE, NULL);
        cchar_t selectedPropertyDotChar;
        setcchar(&selectedPropertyDotChar, L"\uf111", 0, colorGroup + TXT_PURPLE - BGT_PURPLE, NULL);

        unsigned char selectedProperty = 0;
        vector<unsigned char> newHouses(colorGroupProperties.size());

        for (int i = 0; i < colorGroupProperties.size(); i++) {
          newHouses[i] = colorGroupProperties[i]->getHouses();
        }

        short totalNewHouses = 0;
        bool isValidBuild = false;
        short buildingPrice = colorGroupProperties[0]->prices.buildingPrice;
        short currentMoney = player->getBalance();

        updateManageHousesStats(totalNewHouses, buildingPrice, currentMoney, newHouses);

        while (true) {
          // j is now the number of properties we found
          NavigateListResult result = navigateList(static_cast<unsigned char>(colorGroupProperties.size()), selectedProperty);
          if (result == NavigateListResult::Cancel) break;
          else if (result == NavigateListResult::Confirm) {
            // Make sure what they have done is valid, commit changes, and break
            if (isValidBuild) {
              player->alterBalance(-totalNewHouses * buildingPrice, "Houses/hotels");
              for (int i = 0; i < newHouses.size(); i++) {
                colorGroupProperties[i]->setHouses(newHouses[i]);
              }
              doupdate();
              break;
            }
          }
          // Up/down
          else if (result == NavigateListResult::Up || result == NavigateListResult::Down) {
            mvwadd_wch(win, 3 + selectedProperty, 0, &propertyDotChar);
            selectedProperty += result;
            mvwadd_wch(win, 3 + selectedProperty, 0, &selectedPropertyDotChar);
          }
          // Left/Right
          else if (result == NavigateListResult::Left && newHouses[selectedProperty] > 0) {
            // Remove one house and update stats
            newHouses[selectedProperty]--;
            totalNewHouses--;
            isValidBuild = updateManageHousesStats(totalNewHouses, buildingPrice, currentMoney, newHouses);
          } else if (result == NavigateListResult::Right && newHouses[selectedProperty] < 5) {
            newHouses[selectedProperty]++;
            totalNewHouses++;
            isValidBuild = updateManageHousesStats(totalNewHouses, buildingPrice, currentMoney, newHouses);
          }
        }
        break;
      } case 2: {
          // Mortgage/unmortgage properties
          // Mortagable color groups cannot have any houses on them.
          unordered_set<unsigned char> availableColorGroupsSet;

          for (auto currentProperty = player->properties.begin(); currentProperty != player->properties.end(); currentProperty++) {
            // Skip utilities/railroads
            if ((*currentProperty)->colorGroup == BGT_BLACK) continue;

            int colorGroup = (*currentProperty)->colorGroup - BGT_PURPLE;
            unsigned char numHouses = (*currentProperty)->getHouses();
            bool isInSet = availableColorGroupsSet.contains(colorGroup);
            // All houses on the color group cannot have any houses (build evenly)
            bool cannotBeMortgaged = numHouses >= 1 && numHouses <= 5;

            if (cannotBeMortgaged && isInSet) {
              availableColorGroupsSet.erase(colorGroup);
            }
            else if (!(cannotBeMortgaged || isInSet)) {
              availableColorGroupsSet.insert(colorGroup);
            }
          }

          if (availableColorGroupsSet.size() == 0) break;

          // Convert to vector
          vector<unsigned char> availableColorGroups = vector(availableColorGroupsSet.begin(), availableColorGroupsSet.end());

          // Have the player choose a property
          vector<Property*> selectedProperties = promptChooseProperty(availableColorGroups, false, playerId);
          if (selectedProperties.size() == 0) break;
          Property* selectedProperty = selectedProperties[0];

          short mortgageValue = selectedProperty->prices.price / 2;

          // 255 house count means mortgaged
          if (selectedProperty->getHouses() == 255) {
              // Note: We need to make sure they have enough money
              short unmmortgagePrice = (mortgageValue * 11) / 10;
              if (player->getBalance() >= unmmortgagePrice) {
                  selectedProperty->setHouses(0);

                  char unmortgagedCString[100];
                  snprintf(unmortgagedCString, 100, "Unmortgaged %s", selectedProperty->name.c_str());

                  player->alterBalance(-unmmortgagePrice, string(unmortgagedCString));
              }
          } else {
              selectedProperty->setHouses(255);

              char mortgagedCString[100];
              snprintf(mortgagedCString, 100, "Mortgaged %s", selectedProperty->name.c_str());

              player->alterBalance(mortgageValue, string(mortgagedCString));
          }
          doupdate();
          break;
      } case 3: {
        // List all properties
        
        vector<Property*> selectedProperties = promptChooseProperty({ 0, 1, 2, 3, 4, 5, 6, 7 }, false, 255); // all color Groups
        if (selectedProperties.size() == 0) break;
        Property* selectedProperty = selectedProperties[0];

        // Show the property details
        drawSubheader(selectedProperty->name);

        wprintw(
          win,
          "Price: $%d\nMortgage Value: $%d\nBuilding Price: $%d\nRent:\n   $%d\n1  $%d\n2  $%d\n3  $%d\n4  $%d\n   $%d\n",
          selectedProperty->prices.price, selectedProperty->prices.price / 2, selectedProperty->prices.buildingPrice,
          selectedProperty->prices.rent[0], selectedProperty->prices.rent[1], selectedProperty->prices.rent[2],
          selectedProperty->prices.rent[3], selectedProperty->prices.rent[4], selectedProperty->prices.rent[5]
        );
        if (selectedProperty->getHouses() == 5) waddstr(win, "Has Hotel\n");
        else if (selectedProperty->getHouses() == 255) waddstr(win, "Mortgaged\n");
        else if (selectedProperty->getHouses() > 0) wprintw(win, "%d Houses\n", selectedProperty->getHouses());

        if (selectedProperty->ownedBy != 255) {
          cchar_t ownerChar;
          setcchar(&ownerChar, L"\uf4ca", 0, players[selectedProperty->ownedBy].color, NULL);
          wadd_wch(win, &ownerChar);
          wprintw(win, " Owned by %s", players[selectedProperty->ownedBy].name.c_str());
        }

        // Print the house icons
        mvwvline_set(win, 8, 1, Icons::house(), 4);
        // Hotel icon (Surrogate pair so capital U)
        mvwadd_wch(win, 12, 1, Icons::hotel());
        wrefresh(win);
        wgetch(win);
        break;
      } case 4: {
        return true;
      } case 5: {
        return false;
      }
    }
  }
}

bool BoardState::promptJailChoices(unsigned char playerId) {
  Player* player = &players[playerId];
  unsigned char newLocation = 255;

  if (player->turnsInJail >= 3) {
    // Force them to pay the fee
    player->alterBalance(-50, "Jail");
    newLocation = 10;
  }
  else {
    bool showGetOutFree = player->numGetOutOfJailCards > 0;
    char numMenuItems = showGetOutFree ? 3 : 2;

    wmove(win, 2, 0);
    wclrtobot(win);
    wprintw(win, "  Pay $50\n");
    wprintw(win, "  Try Rolling Doubles\n");

    if (showGetOutFree) {
      wprintw(win, "  Use Get Out of Jail Free card\n");
    }

    mvwadd_wch(win, 2, 0, Icons::selectedItem());
    mvwvline_set(win, 3, 0, Icons::unselectedItem(), numMenuItems - 1);

    char selectedItem = 0;

    while (true) {
      wrefresh(win);

      int ch = wgetch(win);

      handleCharInput(ch);

      if (ch == KEY_UP || ch == KEY_DOWN || ch == 'w' || ch == 's') {
        // The menu starts at y index = 2
        // We want to replace the char at x index = 0
        mvwadd_wch(win, 2 + selectedItem, 0, Icons::unselectedItem());

        if (ch == KEY_UP || ch == 'w') selectedItem--;
        else if (ch == KEY_DOWN || ch == 's') selectedItem++; // KEY_DOWN = higher y value because high y is down

        if (selectedItem >= numMenuItems) selectedItem = 0;
        else if (selectedItem < 0) selectedItem = numMenuItems - 1;

        mvwadd_wch(win, 2 + selectedItem, 0, Icons::selectedItem());
      }
      else if (ch == KEY_ENTER || ch == '\n') {
        break;
      }
    }

    wrefresh(win);

    switch (selectedItem) {
    case 0:
      // Pay 50
      player->alterBalance(-50, "Jail");
      newLocation = 10;
      break;
    case 1: {
      // Roll doubles
      unsigned char rolls = rollDice(playerId);
      unsigned char roll1 = rolls & 0b111;
      unsigned char roll2 = rolls >> 3;
      boardCenter.showDiceRoll(roll1, roll2);

      if (roll1 == roll2) {
        // 10 is the index of jail (just visiting)
        newLocation = 10 + roll1 + roll2;
      }
      break;
    }
    case 2:
      // Get out of jail free
      player->numGetOutOfJailCards--;
      newLocation = 10;
      break;
    }
  }
  if (newLocation != 255) {
    jail.releasePlayer(playerId);
    BoardItem::RollInfo info = {
      .roll = static_cast<unsigned char>(newLocation - 10), // 10 is jail (just visiting)
      .isChanceMultiplied = false
    };
    player->boardItemIndex = newLocation;
    boardItems[player->boardItemIndex]->handlePlayer(player, this, &info);
    drawHeader(playerId, boardItems[player->boardItemIndex]->name);

    return true;
  }
  return false;
}

bool BoardState::updateManageHousesStats(short totalNewHouses, short buildingPrice, short currentMoney, vector<unsigned char> newHouses) {
  int minHouses = newHouses[0], maxHouses = newHouses[0];
  for (int i = 0; i < newHouses.size(); i++) {
    wmove(win, 3 + i, 30);
    wclrtoeol(win);
    int houses = newHouses[i];
    
    if (houses >= 1 && houses <= 4) {
      mvwhline_set(win, 3 + i, 30, Icons::house(), houses);
    }
    else if (houses == 5) {
      mvwadd_wch(win, 3 + i, 30, Icons::hotel());
    }

    if (houses < minHouses) minHouses = houses;
    if (houses > maxHouses) maxHouses = houses;
  }

  bool isEvenBuild = (maxHouses - minHouses) <= 1;

  short totalPrice = totalNewHouses * buildingPrice;

  mvwprintw(win, 3 + static_cast<int>(newHouses.size()) + 1, 0, "Price: $%d\n", totalPrice);
  if (!isEvenBuild) {
    wattron(win, COLOR_PAIR(TXT_RED));
    wprintw(win, "You must build evenly\n");
    wattroff(win, COLOR_PAIR(TXT_RED));
  } else if (totalPrice > currentMoney) {
    wattron(win, COLOR_PAIR(TXT_YELLOW));
    wprintw(win, "You don't have enough money\n");
    wattroff(win, COLOR_PAIR(TXT_YELLOW));
  } else {
    wprintw(win, "Press <enter> to commit changes\n");
  }

  wprintw(win, "Press <left> and <right> to alter house count\n");

  wclrtobot(win);

  return isEvenBuild && totalPrice <= currentMoney;
}

void BoardState::drawHeader(unsigned char playerId, string location) {
  wattron(win, A_BOLD | A_UNDERLINE);
  mvwprintw(win, 0, 0, "%s, it's your turn.", players[playerId].name.c_str());
  wclrtoeol(win);
  wattroff(win, A_BOLD | A_UNDERLINE);
  mvwprintw(win, 1, 0, "You are on %s.", location.c_str());
  wclrtobot(win);
}

/// Print the header and clear everything else
void BoardState::drawSubheader(string text) {
  wmove(win, 2, 0);
  wclrtobot(win);
  wattron(win, A_UNDERLINE);
  wprintw(win, "%s:\n", text.c_str());
  wattroff(win, A_UNDERLINE);
}

vector<Property*> BoardState::promptChooseProperty(vector<unsigned char> colorGroups, bool onlyPrintProperties, unsigned char ownedBy) {
  constexpr unsigned char txtColorOffset = TXT_PURPLE;

  constexpr const char* colorGroupNames[] = { "Purple", "Light Blue", "Pink", "Orange", "Red", "Yellow", "Green", "Blue" };
  constexpr unsigned char propertyOffsets[] = { 0, 3, 6, 11, 14, 18, 22, 26 }; // indices of the first properties in each color group

  drawSubheader("Select Color Group");

  cchar_t propertyDotChar;
  setcchar(&propertyDotChar, L"\uf10c", 0, 0, NULL);
  cchar_t selectedPropertyDotChar;
  setcchar(&selectedPropertyDotChar, L"\uf111", 0, colorGroups[0] + txtColorOffset, NULL);

  unsigned char selectedColorGroup = 0;

  // Print the first item (selected) and all the other items
  for (vector<unsigned char>::iterator colorGroup = colorGroups.begin(); colorGroup != colorGroups.end(); colorGroup++) {
    SET_CCHAR_COLOR(propertyDotChar, *colorGroup + txtColorOffset);
    wadd_wch(win, &propertyDotChar);
    wprintw(win, " %s\n", colorGroupNames[*colorGroup]);
  }

  mvwadd_wch(win, 3, 0, &selectedPropertyDotChar);

  // At most three elements
  vector<Property*> resultProperties;

  // Have the user choose a color group
  while (true) {
    NavigateListResult result = navigateList(static_cast<unsigned char>(colorGroups.size()), selectedColorGroup);
    // Cancel
    if (result == NavigateListResult::Cancel) break;
    // Confirm
    else if (result == NavigateListResult::Confirm) {
      // Next, prompt for a property
      drawSubheader("Select Property");
      unsigned char offset = propertyOffsets[colorGroups[selectedColorGroup]];

      SET_CCHAR_COLOR(propertyDotChar, colorGroups[selectedColorGroup] + txtColorOffset);
      vector<Property*> colorGroupProperties;
      colorGroupProperties.reserve(3);


      // Render a list of all of the properties
      // Loop until we reach a property of a different color group
      for (unsigned char i = offset; properties[i].colorGroup - BGT_PURPLE == colorGroups[selectedColorGroup]; i++) {
        // If we need to filter by whether they own it, 
        if (ownedBy != 255 && properties[i].ownedBy != ownedBy) continue;
        // Skip over utilities
        if (properties[i].colorGroup == BGT_BLACK) continue;

        wadd_wch(win, &propertyDotChar);
        wprintw(win, " %s\n", properties[i].name.c_str());
        colorGroupProperties.push_back(&properties[i]);
      }

      mvwadd_wch(win, 3, 0, &selectedPropertyDotChar);

      if (onlyPrintProperties) {
        return colorGroupProperties;
      }

      unsigned char selectedProperty = 0;

      while (true) {
        // j is now the number of properties we found
        NavigateListResult subResult = navigateList(static_cast<unsigned char>(colorGroupProperties.size()), selectedProperty);
        if (subResult == NavigateListResult::Cancel) goto end;
        else if (subResult == NavigateListResult::Confirm) {
          // Add the resulting property and exit;
          resultProperties.push_back(colorGroupProperties[selectedProperty]);
          goto end;
        }
        else if (subResult == NavigateListResult::Up || subResult == NavigateListResult::Down) {
          mvwadd_wch(win, 3 + selectedProperty, 0, &propertyDotChar);
          selectedProperty += static_cast<int>(subResult);
          mvwadd_wch(win, 3 + selectedProperty, 0, &selectedPropertyDotChar);
        }
      }

      wgetch(win);

      break;
    // Up/down
    } else if (result == NavigateListResult::Up || result == NavigateListResult::Down) {
      // Make the previously selected one an open dot
      SET_CCHAR_COLOR(propertyDotChar, colorGroups[selectedColorGroup] + txtColorOffset);
      mvwadd_wch(win, 3 + selectedColorGroup, 0, &propertyDotChar);

      selectedColorGroup += static_cast<int>(result);

      // Make the new one a closed dot
      SET_CCHAR_COLOR(selectedPropertyDotChar, colorGroups[selectedColorGroup] + txtColorOffset);
      mvwadd_wch(win, 3 + selectedColorGroup, 0, &selectedPropertyDotChar);
    }
  }
end: return resultProperties;
}

BoardState::NavigateListResult BoardState::navigateList(unsigned char maxItems, unsigned char currentItem) {
  wrefresh(win);

  int ch = wgetch(win);

  handleCharInput(ch);

  if ((ch == KEY_DOWN || ch == 's') && currentItem < maxItems - 1) {
    return NavigateListResult::Down;
  } else if ((ch == KEY_UP || ch == 'w') && currentItem > 0) {
    return NavigateListResult::Up;
  } else if (ch == KEY_LEFT || ch == 'a') {
    return NavigateListResult::Left;
  } else if (ch == KEY_RIGHT || ch == 'd') {
    return NavigateListResult::Right;
  } else if (ch == KEY_ENTER || ch == '\n') {
    return NavigateListResult::Confirm;
  } else if (ch == 033) { // ESC character
    return NavigateListResult::Cancel;
  } else {
    return NavigateListResult::NoAction;
  }
}

char BoardState::drawMenu(bool showRollDice) {
  char numMenuItems = showRollDice ? NUM_MENU_ITEMS : NUM_MENU_ITEMS - 1;

  wmove(win, 2, 0);
  wclrtobot(win);
  if (showRollDice) {
    wprintw(win, "  Roll Dice\n");
  }
  wprintw(win, "  Manage Houses/Hotels\n");
  wprintw(win, "  (Un)Mortage Properties\n");
  wprintw(win, "  View Property Info\n");
  wprintw(win, "  End Game\n");
  wprintw(win, "  End Turn");

  mvwadd_wch(win, 2, 0, Icons::selectedItem());
  mvwvline_set(win, 3, 0, Icons::unselectedItem(), numMenuItems - 1);

  char selectedItem = 0;

  while (true) {
    wrefresh(win);

    int ch = wgetch(win);

    handleCharInput(ch);

    if (ch == KEY_UP || ch == KEY_DOWN || ch == 'w' || ch == 's' || (ch >= '1' && ch <= '0' + numMenuItems)) {
      // The menu starts at y index = 2
      // We want to replace the char at x index = 0
      mvwadd_wch(win, 2 + selectedItem, 0, Icons::unselectedItem());

      if (ch == KEY_UP || ch == 'w') selectedItem--;
      else if (ch == KEY_DOWN || ch == 's') selectedItem++; // KEY_DOWN = higher y value because high y is down
      else selectedItem = ch - '1';

      if (selectedItem >= numMenuItems) selectedItem = 0;
      else if (selectedItem < 0) selectedItem = numMenuItems - 1;

      mvwadd_wch(win, 2 + selectedItem, 0, Icons::selectedItem());
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
unsigned char BoardState::rollDice(unsigned char playerId) {
  auto roll1 = dice(mt);
  auto roll2 = dice(mt);

  // Each roll is from 1 to 6, which can be represented with 3 bits
  // This combines the two rolls into one byte. Roll 1 is the first 3 bytes, roll 2 is the next 3
  return static_cast<unsigned char>((roll1 & 0b111) | ((roll2 & 0b111) << 3));
}

void BoardState::movePlayerTo(unsigned char playerId, unsigned char boardItemIndex, BoardItem::RollInfo* rollInfo) {
  Player* player = &players[playerId];
  boardItems[player->boardItemIndex]->handlePlayerLeave(playerId);

  // If the new board item index is less than their current index, that means they had to go all the way around
  if (boardItemIndex <= player->boardItemIndex) {
    // Give the salary
    player->alterBalance(200, "Salary");
  }

  player->boardItemIndex = boardItemIndex;

  mvwprintw(win, 1, 0, "You are on %s.", boardItems[player->boardItemIndex]->name.c_str());
  wclrtoeol(win);
  wnoutrefresh(win);
  boardItems[player->boardItemIndex]->handlePlayer(player, this, rollInfo);
}

unsigned short BoardState::drawChance(RandomDraw::RandomDrawType type) {
    return chanceDrawer(mt);
}

void BoardState::showChanceDraw(unsigned char playerId, RandomDraw::RandomDrawType type, BoardItem::RollInfo* rollInfo) {
  unsigned short i = drawChance(type);
  unsigned char pos = players[playerId].boardItemIndex;
  constexpr unsigned char NUM_BOARD_ITEMS = static_cast<unsigned char>(sizeof(boardItems) / sizeof(BoardItem*));

  if (type == RandomDraw::Chance) {
    boardCenter.showChanceDraw(chanceMessages[i], type);

    switch (i) {
      case 0:
        // Boardwalk
        pos = 39;
        break;
      case 1:
        // Go
        pos = 0;
        break;
      case 2:
        // Illinois
        pos = 24;
        break;
      case 3:
        // St. Charles
        pos = 11;
        break;
      case 4:
      case 5: {
        // Nearest railroad
        unsigned char spacesToRailroad =
          10 - // subtract to get distance to
          ((players[playerId].boardItemIndex + 5) // offset so that railroads appear at multiples of 10
            % 10); // mod 10 gives the distance back

        pos = players[playerId].boardItemIndex + spacesToRailroad;
        if (pos >= NUM_BOARD_ITEMS) pos -= NUM_BOARD_ITEMS;

        rollInfo->isChanceMultiplied = true; // pay 2x rent
        break;
      } case 6: {
        // Nearest utility

        // If they in [electric company, water works) then they should go to water works. Otherwise electric company
        pos = (pos >= 12 && pos < 28) ? 28 : 12;

        rollInfo->isChanceMultiplied = true; // pay 10x dice roll
        break;
      } case 7:
        // Bank pays $50
        players[playerId].alterBalance(+50, "Bank dividend");
        break;
      case 8:
        players[playerId].numGetOutOfJailCards++;
        break;
      case 9:
        // Go to jail
        // Because 255 is bigger than every board item index, the salary won't be triggered
        pos = 255;
        break;
      case 10: {
        // Go back 3 spaces (bypass go unless we actually pass it???)
        boardItems[pos]->handlePlayerLeave(playerId);

        // If pos is less than 3, add the number of board items 
        unsigned char newPos = (pos < 3 ? pos + NUM_BOARD_ITEMS : pos) - 3;

        // If the new new pos is greater (they wrapped around), give the salary
        if (newPos > pos) players[playerId].alterBalance(200, "Salary");

        players[playerId].boardItemIndex = newPos;
        boardItems[newPos]->handlePlayer(&players[playerId], this, rollInfo);
        pos = newPos; // skip the check below
        break;
      } case 11: {
        // Make repairs ($25/$100)
        short totalCharge = 0;

        for (vector<Property*>::iterator it = players[playerId].properties.begin(); it != players[playerId].properties.end(); it++) {
          unsigned char houses = (*it)->getHouses();
          if (houses == 5) totalCharge -= 100;
          else if (houses > 0 && houses < 5) totalCharge -= houses * 25;
        }

        players[playerId].alterBalance(totalCharge, "Repairs");
        break;
      } case 12:
        // Poor tax
        players[playerId].alterBalance(-15, "Poor tax");
        break;
      case 13:
        // Reading Railroad
        pos = 5;
        break;
      case 14: {
        // Chairman

        for (vector<Player>::iterator it = players.begin(); it != players.end(); it++) {
          if (it->id != playerId) {
            it->alterBalance(+50, "From chairman");
          }
        }

        // Total cost is 50 * (numPlayers - 1)
        players[playerId].alterBalance(-50 * static_cast<short>(players.size() - 1), "Elected chairman");
        break;
      } case 15:
        // Building loan
        players[playerId].alterBalance(+150, "Building loan");
        break;
    }
  } else {
    boardCenter.showChanceDraw(communityChestMessages[i], type);

    switch (i) {
      case 0:
        players[playerId].alterBalance(+100, "Life insurance");
        break;
      case 1:
        players[playerId].alterBalance(+45, "Stock");
        break;
      case 2:
        players[playerId].alterBalance(+100, "Xmas fund");
        break;
      case 3:
        players[playerId].alterBalance(-100, "Hospital fee");
        break;
      case 4:
        players[playerId].numGetOutOfJailCards++;
        break;
      case 5:
        players[playerId].alterBalance(-150, "School tax");
        break;
      case 6:
        // Go to jail
        // Because 255 is bigger than every board item index, the salary won't be triggered
        pos = 255;
        break;
      case 7:
        // Go
        pos = 0;
        break;
      case 8: {
        // Opera opening

        for (vector<Player>::iterator it = players.begin(); it != players.end(); it++) {
          if (it->id != playerId) {
            it->alterBalance(-50, "Opera opening");
          }
        }

        // Total is 50 * (numPlayers - 1)
        players[playerId].alterBalance(+50 * static_cast<short>(players.size() - 1), "Opera opening");
        break;
      }
      case 9:
        players[playerId].alterBalance(+20, "Income tax refund");
        break;
      case 10:
        players[playerId].alterBalance(+25, "Services");
        break;
      case 11:
        players[playerId].alterBalance(+100, "Inheritance");
        break;
      case 12:
        players[playerId].alterBalance(+200, "Bank error");
        break;
      case 13:
        players[playerId].alterBalance(+10, "Beauty contest");
        break;
      case 14:
        players[playerId].alterBalance(-50, "Doctor's fee");
        break;
      case 15: {
        // Street repairs ($40/$115)
        short totalCharge = 0;

        for (vector<Property*>::iterator it = players[playerId].properties.begin(); it != players[playerId].properties.end(); it++) {
          unsigned char houses = (*it)->getHouses();
          if (houses == 5) totalCharge -= 115;
          else if (houses > 0 && houses < 5) totalCharge -= houses * 40;
        }

        players[playerId].alterBalance(totalCharge, "Street repairs");
        break;
      }
    }
  }

  // If their position should change, update it
  if (pos != players[playerId].boardItemIndex) {
    movePlayerTo(playerId, pos, rollInfo);
  }
}
