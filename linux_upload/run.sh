#!/bin/bash

SRC_DIR=`pwd`
echo $SRC_DIR

export QT_QWS_FONTDIR=$SRC_DIR/fonts

LD_LIBRARY_PATH=./ ./inspection_module
