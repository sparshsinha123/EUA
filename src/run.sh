#!/bin/bash

g++ -DLOCAL euaSolver.cpp -o euaSolver
./euaSolver $1 > output.txt
echo "output generated in output.txt"