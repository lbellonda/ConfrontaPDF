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


#ifndef COMPARERESULTS_H
#define COMPARERESULTS_H

#include "generic.hpp"

class CompareResults
{
    int _total;
    int _count;
    QList<PagePair> _differences ;
public:
    CompareResults();
    ~CompareResults();
    int total() const;
    void setTotal(int total);
    int count() const;
    void incCount();
    QList<PagePair> &differences();
};

#endif // COMPARERESULTS_H
