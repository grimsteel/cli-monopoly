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
send "d" "\n"

sleep 1
# Roll dice
send "\n"
sleep 1
# Accept
send "\n"

# End turn, Roll dice
send "w" "\n" "\n"
sleep 1
# Accept
send "\n"
# Roll dice
send "\n"
sleep 1
# Accept
send "\n"

# End turn, Roll dice
send "w" "\n" "\n"
sleep 1
# Roll dice
send "\n"
sleep 1
# Accept
send "\n"

# End turn, Roll dice
send "w" "\n" "\n"
sleep 1

# End turn, Roll dice
send "w" "\n" "\n"
sleep 1

# End turn, Roll dice
send "w" "\n" "\n"
sleep 1
# Accept
send "\n"

sleep 1

pkill CmdLineMonopoly
