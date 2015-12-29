#!/bin/sh
qmake confrontapdf.pro "CONFIG+=release"
make
qmake confrontapdfc.pro "CONFIG+=release"
make

