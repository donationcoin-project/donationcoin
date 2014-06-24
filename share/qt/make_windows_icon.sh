#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/donationcoin.png
ICON_DST=../../src/qt/res/icons/donationcoin.ico
convert ${ICON_SRC} -resize 16x16 donationcoin-16.png
convert ${ICON_SRC} -resize 32x32 donationcoin-32.png
convert ${ICON_SRC} -resize 48x48 donationcoin-48.png
convert donationcoin-16.png donationcoin-32.png donationcoin-48.png ${ICON_DST}

