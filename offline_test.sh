#!/bin/sh
#still in development
g++ -o offline_test -I/usr/include/unity -L/usr/lib/unity -lunity ./src/UnitTests/TestsMain.cpp
