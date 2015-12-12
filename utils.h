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


#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include "startupparameters.h"

class Utils
{
    // private constructor
    Utils();
public:
    static int validateInt(const QString &input, const int defaultValue, bool *error);
    static EReturnType validateReturnType(const QString &input, const EReturnType defaultValue, bool *error);

};

#endif // UTILS_H
