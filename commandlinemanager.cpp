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


#include "sequence_matcher.hpp"
#include "textitem.hpp"
#ifdef DEBUG
#include <QtDebug>
#endif
#include <QCoreApplication>
#include "commandlinemanager.h"

CommandLineManager::CommandLineManager(const Debug debug,
                                       const InitialComparisonMode comparisonMode,
                                       const QString &filename1, const QString &filename2,
                                       StartupParameters *startupParameters, Status *status )
    : compare(debug,
              comparisonMode,
              filename1, filename2,
              startupParameters, status)
{
    compare.setNotifier(this);
}

CommandLineManager::~CommandLineManager()
{
}


void CommandLineManager::initCompareParams()
{
    compare.readFromSettings();
}

void CommandLineManager::batchOperation()
{
    initCompareParams();
    compare.batchOperation();
}

BatchCompare *CommandLineManager::getCompare()
{
    return &compare;
}


//---- begin notifier interface
void CommandLineManager::writeError(const QString & /*text*/){}
void CommandLineManager::writeLine(const QString & /*text*/){}
void CommandLineManager::setOverrideCursor(){}
void CommandLineManager::setRestoreCursor(){}
void CommandLineManager::processEvents()
{
    QCoreApplication::processEvents();
}

void CommandLineManager::setStatusLabel(const QString & /*text*/){}
void CommandLineManager::messageBox(const QString & /*text */ ){}


//-- end notifier interface
