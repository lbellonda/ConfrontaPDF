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


#ifndef DOCINFO_H
#define DOCINFO_H

#include <QString>
#include <QList>
#include <QPair>


class LFontInfo {
public:
    QString name;
    bool subset;
    bool embedded;
    QString typeName;
    LFontInfo();
};

class DocInfo
{
public:
    int pdfVersionMajor;
    int pdfVersionMinor;
    QString fileName;
    int pageCount;
    QString creationDate;
    QString modDate;
    QString pageSize;
    QList< QPair< QString, QString> > infos;
    QList< LFontInfo *> fonts;

    DocInfo();
    ~DocInfo();
};

#endif // DOCINFO_H
