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


#include "docinfo.h"

LFontInfo::LFontInfo()
{
    embedded = false;
    subset = false;
}

DocInfo::DocInfo()
{
}

DocInfo::~DocInfo()
{
    while( fonts.size() > 0 ) {
        LFontInfo *fi = fonts.first();
        fonts.removeFirst();
        delete fi;
    }
}
