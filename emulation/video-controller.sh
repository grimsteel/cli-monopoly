#!/bin/bash

send() {
  for char in "$@"; do
    echo -ne $char > video.fifo
    sleep 0.1
  done
}

# Initial prompt
echo > video.fifo
sleep 0.25

send "A" "l" "i" "c" "e" "\n"
send "d" "d" "d" "d" "\n"
sleep 0.5
send "\n"
sleep 0.5
send "B" "o" "b" "\n"
send "d" "d" "d" "d" "d" "\n"
sleep 0.5
# no more players
send "d" "\n"

sleep 1
# Roll dice (to Chance, to Reading)
send "\n"
sleep 2
# Buy Reading
send "\n"

sleep 1

# End turn, Roll dice (to Electric)
send "w" 
sleep 1
send "\n" "\n"
sleep 2
# Don't buy Electric
send "d" "\n"
sleep 1
# Roll dice (To Penn)
send "\n"
sleep 1
# Buy Penn
send "\n"

sleep 1

# End turn, Roll dice (to Penn)
send "w" 
sleep 1
send "\n" "\n"
sleep 2
# Roll dice (to B & O)
send "\n"
sleep 1
# Buy B & O
send "\n"

sleep 1

# End turn, Roll dice (to Atlantic)
send "w" 
sleep 1
send "\n" "\n"
sleep 2
# Do not buy Atlantic
send "d" "\n" 

sleep 1

# End turn, Roll dice (to Jail)
send "w" 
sleep 1
send "\n" "\n"
sleep 1

# End turn, Roll dice (to Park Place)
send "w" 
sleep 1
send "\n" "\n"
sleep 2
# Buy Park Place
send "\n"

sleep 1

# End turn
send "w" "\n"
sleep 1
# Pay 50, Roll Dice (to Electric)
send "\n" "\n"
sleep 2
# Don't buy electric
send "d" "\n"

sleep 1

# End turn
send "w" "\n" "s" "s" "s"
sleep 0.5
# View Property Info -> Blue
send "\n" "s" "s" "s" "s" "s" "s" "s"
sleep 0.5
# Boardwalk
send "\n" "s"
sleep 0.5
send "\n"
sleep 3
# Exit
send "\n"
# Roll Dice -> to boardwalk
send "\n"
sleep 2
# Buy boardwalk
send "\n"
sleep 1
# Roll dice (to Reading)
send "\n"
sleep 0.5
# Buy houses
send "\n"
sleep 0.5
send "\n" "d" "s" "d" "d"
sleep 1
send "\n"

# End turn, Roll dice (to chance, to boardwalk)
send "w" "\n" 
sleep 1
send "\n"


sleep 3

pkill CmdLineMonopoly
