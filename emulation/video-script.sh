#!/bin/bash

bin/CmdLineMonopoly &
pid=$!
#echo asd > /proc/$pid/fd/0
#echo asd > /proc/$pid/fd/0
#echo asd > /proc/$pid/fd/0
sleep 5
kill $pid