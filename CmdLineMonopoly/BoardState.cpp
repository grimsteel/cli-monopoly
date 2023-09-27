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
    Property(0, "Mediterranean Avenue",   " Mediterran ean Avenue", 60,  BGT_PURPLE,  BoardItem::Bottom),
    Property(2, "Baltic Avenue",          "   Baltic     Avenue  ", 60,  BGT_PURPLE,  BoardItem::Bottom),
    Property(4, "Reading Railroad",       "   Reading   Railroad ", 200, BGT_BLACK,   BoardItem::Bottom),
    Property(5, "Oriental Avenue",        "  Oriental    Avenue  ", 100, BGT_LBLUE,   BoardItem::Bottom),
    Property(7, "Vermont Avenue",         "  Vermont     Avenue  ", 100, BGT_LBLUE,   BoardItem::Bottom),
    Property(8, "Connecticut Avenue",     " Connecticu  t Avenue ", 120, BGT_LBLUE,   BoardItem::Bottom),

    Property(0, "St. Charles Place",      " St Charles Pl.",        140, BGT_PINK,    BoardItem::Left),
    Property(1, "Electric Company",       "  Electric Co. ",        150, BGT_BLACK,   BoardItem::Left),
    Property(2, "States Avenue",          " States Avenue ",        140, BGT_PINK,    BoardItem::Left),
    Property(3, "Virginia Avenue",        " Virginia Ave. ",        150, BGT_PINK,    BoardItem::Left),
    Property(4, "Pennsylvania Railroad",  " Penn. Railroad",        200, BGT_BLACK,   BoardItem::Left),
    Property(5, "St. James Place",        " St. James Pl. ",        180, BGT_ORANGE,  BoardItem::Left),
    Property(7, "Tennessee Avenue",       " Tennessee Ave.",        180, BGT_ORANGE,  BoardItem::Left),
    Property(8, "New York Avenue",        " New York Ave. ",        200, BGT_ORANGE,  BoardItem::Left),

    Property(0, "Kentucky Avenue",        "  Kentucky    Avenue  ", 220, BGT_RED,     BoardItem::Top),
    Property(2, "Indiana Avenue",         "  Indiana     Avenue  ", 220, BGT_RED,     BoardItem::Top),
    Property(3, "Illinois Avenue",        "  Illinois    Avenue  ", 240, BGT_RED,     BoardItem::Top),
    Property(4, "B. & O. Railroad",       "   B. & O.   Railroad ", 200, BGT_BLACK,   BoardItem::Top),
    Property(5, "Atlantic Avenue",        "  Atlantic    Avenue  ", 260, BGT_YELLOW,  BoardItem::Top),
    Property(6, "Ventnor Avenue",         "  Ventnor     Avenue  ", 260, BGT_YELLOW,  BoardItem::Top),
    Property(7, "Water Works",            "   Water      Works   ", 150, BGT_BLACK,   BoardItem::Top),
    Property(8, "Marvin Gardens",         "   Marvin    Gardens  ", 280, BGT_YELLOW,  BoardItem::Top),

    Property(0, "Pacific Avenue",         "  Pacific Ave. ",        300, BGT_GREEN,   BoardItem::Right),
    Property(1, "North Carolina Avenue",  " N Carolina Ave",        300, BGT_GREEN,   BoardItem::Right),
    Property(3, "Pennsylvania Avenue",    "  Penn. Avenue ",        320, BGT_GREEN,   BoardItem::Right),
    Property(4, "Short Line",             "   Short Line  ",        200, BGT_BLACK,   BoardItem::Right),
    Property(6, "Park Place",             "   Park Place  ",        350, BGT_BLUE,    BoardItem::Right),
    Property(8, "Boardwalk",              "   Boardwalk   ",        400, BGT_BLUE,    BoardItem::Right)
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
  win = newwin(10, 30, 1, H_PROPERTY_WIDTH * 2 + V_PROPERTY_WIDTH * PROPERTIES_PER_SIDE + 2);
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

  doTurn(1);
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

void BoardState::doTurn(unsigned char playerId) {
  Player player = players[playerId];
  BoardItem* boardItem = boardItems[player.boardItemIndex];
  bool canRollAgain = false;
  while (true) {
    char result = drawMenu(playerId, boardItem->name);
    switch (result) {
    case 0:
      if (!canRollAgain) {
        // Show error message
        break;
      }
      // Roll dice
      unsigned char rolls = rollDice();
      unsigned char roll1 = rolls & 0b111;
      unsigned char roll2 = rolls >> 3;
      unsigned char total = roll1 + roll2;
      unsigned char newBoardItemIndex = (player.boardItemIndex + total) % (sizeof(boardItems) / sizeof(BoardItem*));
      boardItem->handlePlayerLeave(playerId);
      player.boardItemIndex = newBoardItemIndex;
      boardItem = boardItems[newBoardItemIndex];
      boardItem->handlePlayer(&player, this);
      canRollAgain = roll1 == roll2;
      break;
    case 4:
      goto end_turn;
    }
  }
end_turn:
}

char BoardState::drawMenu(unsigned char playerId, string location) {
  wattron(win, A_BOLD | A_UNDERLINE);
  mvwprintw(win, 0, 0, "%s, it's your turn.", players[playerId].name.c_str());
  wattroff(win, A_BOLD | A_UNDERLINE);
  mvwprintw(win, 1, 0, "You are on %s.", location.c_str());

  wprintw(win, "\n[x] Roll Dice");
  wprintw(win, "\n[ ] Buy Houses/Hotels");
  wprintw(win, "\n[ ] Mortage Properties");
  wprintw(win, "\n[ ] View Property Info");
  wprintw(win, "\n[ ] End Turn");
  wprintw(win, "\n[ ] End Game");

  char selectedItem = 0;

  curs_set(FALSE);

  while (true) {
    wrefresh(win);

    int ch = wgetch(win);

    handleCharInput(ch);

    if (ch == KEY_UP || ch == KEY_DOWN) {
      // Replace the 'x' of the currently selected item with a space
      // The menu starts at y index = 2
      // We want to replace the char at x index = 1
      mvwaddch(win, 2 + selectedItem, 1, ' ');

      if (ch == KEY_UP) selectedItem--;
      else selectedItem++; // KEY_DOWN = higher y value because high y is down

      if (selectedItem >= NUM_MENU_ITEMS) selectedItem = 0;
      else if (selectedItem < 0) selectedItem = NUM_MENU_ITEMS - 1;

      mvwaddch(win, 2 + selectedItem, 1, 'x');
    }
    else if (ch == KEY_ENTER || ch == '\n') {
      break;
    }
  }

  curs_set(FALSE);

  wrefresh(win);

  return selectedItem;
}
bool BoardState::setYesNoPrompt(string prompt) {
  return showYesNoPrompt(win, this, prompt, 0, 2);
}
unsigned char BoardState::rollDice() {
  auto roll1 = dice(mt);
  auto roll2 = dice(mt);

  // Each roll is from 1 to 6, which can be represented with 3 bits
  // This combines the two rolls into one byte. Roll 1 is the first 3 bytes, roll 2 is the next 3
  return static_cast<unsigned char>((roll1 & 0b111) | ((roll2 & 0b111) << 3));
}
