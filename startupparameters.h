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


#ifndef STARTUPPARAMETERS_H
#define STARTUPPARAMETERS_H

#include <QString>
#include "generic.hpp"

class Status ;

enum EReturnType {
    ReturnOnlyCode,
    ReturnCodeAndDescritpion
};

class StartupParameters
{
    int _pages;
    int _startPage1;
    int _startPage2;
    bool _isBatch;
    EReturnType _returnType;
    bool _enablePDFDiff;
    bool _pdfDiffAllPages ;
    QString _pdfDiffFilePath;
    QString _file1;
    QString _file2;
    InitialComparisonMode _comparisonMode;
public:
    StartupParameters();
    ~StartupParameters();

    bool parseArgument(const QString &arg, Status *status);
    bool validate(Status *status);

    const static int AllPages = -1 ;


    EReturnType returnType() const;
    void setReturnType(const EReturnType &returnType);
    bool isBatch() const;
    void setIsBatch(bool isBatch);
    QString file1() const;
    void setFile1(const QString &file1);
    QString file2() const;
    void setFile2(const QString &file2);

    int pages() const;
    void setPages(int pages);
    int startPage1() const;
    void setStartPage1(int startPage1);
    int startPage2() const;
    void setStartPage2(int startPage2);
    bool enablePDFDiff() const;
    void setEnablePDFDiff(bool enablePDFDiff);
    QString pdfDiffFilePath() const;
    void setPdfDiffFilePath(const QString &pdfDiffFilePath);
    InitialComparisonMode comparisonMode() const;
    void setComparisonMode(const InitialComparisonMode &comparisonMode);
};

#endif // STARTUPPARAMETERS_H
