#!/usr/bin/bash

tail -f video.fifo | bin/CmdLineMonopoly emulation/video-save.txt
