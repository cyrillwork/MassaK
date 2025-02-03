#!/bin/bash
cd ./lib
qmake
make -j4

cd ../inspection_module/
qmake
make -j4

cd ../
qmake
make -j4
