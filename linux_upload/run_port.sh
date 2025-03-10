#!/bin/bash

SRC_DIR=`pwd`
echo $SRC_DIR

export QT_QWS_FONTDIR=$SRC_DIR/fonts
export QT_QPA_FONTDIR=$SRC_DIR/fonts

./inspection_module -p $@