#!/bin/bash

cd ./lib
mkdir ./build
cd ./build
cmake ../
make -j4
cd ../../
mkdir ./build
cd ./build
cmake ../
make -j4


