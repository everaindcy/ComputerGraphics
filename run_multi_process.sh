#! /bin/bash
g++ -O2 -fopenmp -std=c++17 ./src/main_multi_process.cc -o ./bin/main_multi_process
./bin/main_multi_process > output/$1.ppm