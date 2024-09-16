# Command Line Monopoly in C++

>  AP CSP: 2023 - 2024

This project will have two main parts:
1. A common Monopoly library for managing game state and simulating actions
2. A console interface to Monopoly using `ncurses` (Linux) or `pdcurses` (Windows).

### Progress:

- [x] Monopoly Library
  - [x] Properties
  - [x] Money dealing (rent)
  - [x] Houses
  - [x] Chance/Community Chest
- [x] Console Interface
  - [x] Properties
  - [x] Menu
  - [x] Stats
- [x] Create Task Submittal Generation

## Building

`make`: Compile and link entire project

### Misc

`make screenshots`: Create a PDF file of the entire project along with PNG files of individual segments (these PNG files do not include any comments).

**Video**:

```shell
mkfifo video.fifo
# This records the current terminal
asciinema rec video.cast
emulation/video-game.sh

# In another shell:
# This controls the game through video.fifo
emulation/video-controller.sh
```

## About

### Structure of a turn:

1. Player is prompted with a menu. They may either:
    1. Roll dice
    1. End turn
    1. Buy houses/hotels
    1. Mortgage properties
1. If the player opts to roll the dice, they are moved a random number of spaces forward.
1. If the player passed Go, $200 is added to their balance.
1. The board item's `handlePlayer` method is called.
    This method makes the player's icon appear on the property, and takes any other actions:
    1. Chance/community chest -> draw card and take action
    1. Tax -> deduct money
    1. Go to Jail
    1. Property -> buy if enough money or pay rent
    1. Free Parking/Jail/Go -> do nothing
1. If any of these actions reduced the player's balance to 0, they are declared bankrupt and cannot particpate further
1. Next, the menu is presented again. The option to roll a dice will only appear if they rolled doubles
1. If they roll doubles 3 times, they immediately go to jail, and are presented with the menu again

### Property Value Calculation

[Wikibooks](https://en.wikibooks.org/wiki/Monopoly/Properties_reference)

There does not seem to be a visible pattern.
For some properties, 2 house rent = 3 * 1 house rent
