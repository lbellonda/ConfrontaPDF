#!/bin/sh
lrelease confrontapdf.pro
qmake confrontapdf.pro "CONFIG+=release"
make
lrelease confrontapdfc.pro
qmake confrontapdfc.pro "CONFIG+=release"
make

