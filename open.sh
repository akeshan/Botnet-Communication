#!/bin/sh
g++ -g -std=c++11 -pthread botnet.cpp -o botnet
gcc server.c -o server
./server &
xterm -hold -title "botnet1" -e "./botnet 7" &
xterm -hold -title "botnet2" -e "./botnet 8" &
xterm -hold -title "botnet3" -e "./botnet 3" &
xterm -hold -title "botnet4" -e "./botnet 4" &

