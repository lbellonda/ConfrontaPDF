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

#ifndef COMMANDLINEMANAGER_H
#define COMMANDLINEMANAGER_H

#if QT_VERSION >= 0x040600
#include <QSharedPointer>
#else
#include <tr1/memory>
#endif
#if defined(USE_QT5)
   #include <poppler-qt5.h>
#else
    #include <poppler-qt4.h>
#endif
#include <QBrush>
#include <QList>
#include <QPen>
#include "startupparameters.h"
#include "status.h"
#include "compareresults.h"
#include "batchcompare.h"

class CommandLineManager : public CompareNotifier
{
    BatchCompare compare;

    void initCompareParams();
public:
    CommandLineManager(const Debug debug,
                       const InitialComparisonMode comparisonMode,
                       const QString &filename1, const QString &filename2,
                       StartupParameters *startupParameters, Status *status);
    ~CommandLineManager();

    void writeError(const QString &text);
    void writeLine(const QString &text);
    void setOverrideCursor();
    void setRestoreCursor();
    void processEvents();
    void setStatusLabel(const QString &text);
    void messageBox(const QString & /*text */ );

    void batchOperation();

    BatchCompare *getCompare();
};

#endif // COMMANDLINEMANAGER_H
