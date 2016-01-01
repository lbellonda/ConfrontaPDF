lrelease confrontapdf.pro
qmake confrontapdf.pro "CONFIG+=release"
mingw32-make
lrelease confrontapdfc.pro
qmake confrontapdfc.pro "CONFIG+=release"
mingw32-make

