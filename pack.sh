#!/bin/bash

mkdir ../inspection_module
mkdir ../inspection_module/lib
mkdir ../inspection_module/lib/serial

cp ./*.cpp ../inspection_module
cp ./*.h   ../inspection_module
cp ./*.pro ../inspection_module
cp ./*.png ../inspection_module
cp ./*.ui  ../inspection_module

cp ./build.sh ../inspection_module
cp ./run.sh ../inspection_module
cp ./clean.sh ../inspection_module

cp ./lib/*.cpp ../inspection_module/lib
cp ./lib/*.h   ../inspection_module/lib
cp ./lib/*.hpp ../inspection_module/lib
cp ./lib/*.pro ../inspection_module/lib

cp ./lib/serial/*.cpp ../inspection_module/lib/serial
cp ./lib/serial/*.h   ../inspection_module/lib/serial


