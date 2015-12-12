/*
    Copyright © 2015 Luca Bellonda. All rights reserved.
    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 2 of
    the License, or (at your option) any later version. This program is
    distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.
*/


#include "compareresults.h"


CompareResults::CompareResults()
{
    _total = 0;
    _count = 0 ;
}

CompareResults::~CompareResults()
{
}


int CompareResults::total() const
{
    return _total;
}

void CompareResults::setTotal(int total)
{
    _total = total;
}

int CompareResults::count() const
{
    return _count;
}

void CompareResults::incCount()
{
    _count ++;
}

QList<PagePair> &CompareResults::differences() {
    return _differences;
}

