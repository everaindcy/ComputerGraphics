#! /bin/bash
g++ -std=c++17 ./src/main.cc -o ./bin/main
./bin/main > output/$1.ppm