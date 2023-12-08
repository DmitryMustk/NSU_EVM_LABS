#!/bin/bash

mkdir -p o_builds && cd o_builds
g++  -O0 ../src/main.cpp -o task1_O0
g++  -O1 ../src/main.cpp -o task1_O1
g++  -O2 ../src/main.cpp -o task1_O2
g++  -O3 ../src/main.cpp -o task1_O3
g++  -Os ../src/main.cpp -o task1_Os
g++  -Ofast ../src/main.cpp -o task1_Ofast
g++  -Og ../src/main.cpp -o task1_Og
