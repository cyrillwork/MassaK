#!/bin/bash
cd ./lib
make distclean
rm -R -f ./build

cd ../inspection_module/
make distclean
rm -R -f ./build

cd ../
make distclean
rm -R -f ./build

