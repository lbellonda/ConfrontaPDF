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

#include "startupparameters.h"
#include "status.h"
#include "utils.h"

StartupParameters::StartupParameters()
{
    _startTime = QDateTime::currentDateTime();
    _pages = AllPages ;
    _startPage1 = 1 ;
    _startPage2 = 1 ;
    _isBatch = false ;
    _returnType = ReturnOnlyCode ;
    _enablePDFDiff = false ;
    _pdfDiffAllPages = false ;
    _useXmlResult = false;
}

StartupParameters::~StartupParameters()
{
}

const QString StartupParameters::BatchExtended = "--batch" ;
const QString StartupParameters::Batch = "-b" ;
static const QString OutType = "--outType=" ;
static const QString Pages = "--pages=" ;
static const QString StartPage1 = "--startPage1=";
static const QString StartPage2 = "--startPage2=";
static const QString PDFDiffPath = "--pdfdiff=";
static const QString XMLResultFile = "--xmlResult=";
static const QString Key = "--key=";
static const QString SettingsFile = "--settings=";

bool StartupParameters::parseArgument(const QString &arg, Status *status)
{
    bool error = false ;
    if( ( arg == BatchExtended) || (arg == Batch) ) {
        _isBatch = true ;
    } else if( arg.startsWith(OutType)) {
        _returnType = Utils::validateReturnType(arg.mid(OutType.length()), _returnType, &error);
        status->setParamError(error, OutType);
    } else if( arg.startsWith(Pages)) {
        _pages = Utils::validateInt(arg.mid(Pages.length()), _pages, &error );
        if( _pages < -1 ) {
            error = true ;
        }
        status->setParamError(error, Pages);
    } else if( arg.startsWith(StartPage1)) {
        _startPage1 = Utils::validateInt(arg.mid(StartPage1.length()), _startPage1, &error );
        if( _startPage1 < 1 ) {
            error = true ;
        }
        status->setParamError(error, StartPage1);
    } else if( arg.startsWith(StartPage2)) {
        _startPage2 = Utils::validateInt(arg.mid(StartPage2.length()), _startPage2, &error );
        if( _startPage2 < 1 ) {
            error = true ;
        }
        status->setParamError(error, StartPage2);
    } else if( arg.startsWith(PDFDiffPath)) {
        _pdfDiffFilePath = arg.mid(PDFDiffPath.length());
        if(!_pdfDiffFilePath.isEmpty() ) {
            _enablePDFDiff = true ;
        } else {
          error = true ;
        }
        status->setParamError(error, PDFDiffPath);
    } else if( arg.startsWith(XMLResultFile)) {
        _xmlResultFile = arg.mid(XMLResultFile.length());
        if(!_xmlResultFile.isEmpty() ) {
            _useXmlResult = true ;
        } else {
            error = true ;
        }
        status->setParamError(error, XMLResultFile);
    } else if( arg.startsWith(Key)) {
        _key = arg.mid(Key.length());
    } else if( arg.startsWith(SettingsFile)) {
        _settingsFile = arg.mid(SettingsFile.length());
        if(_settingsFile.isEmpty() ) {
            error = true ;
        }
        status->setParamError(error, SettingsFile);
    } else {
        return false;
    }
    return true ;
}


bool StartupParameters::validate(Status *status)
{
    if( _isBatch ) {
        if( _file1.isEmpty() ) {
            status->setParamError(true, QObject::tr("file 1 missing"));
            return false;
        }
        if( _file2.isEmpty() ) {
            status->setParamError(true, QObject::tr("file 2 missing"));
            return false;
        }
        if( _file2 == _file1 ) {
            status->setStatusWithDescription(ErrorSameFile, QObject::tr("file: '%1'").arg(_file1));
            return false;
        }
    }
    return true;
}

EReturnType StartupParameters::returnType() const
{
    return _returnType;
}

void StartupParameters::setReturnType(const EReturnType &returnType)
{
    _returnType = returnType;
}

bool StartupParameters::isBatch() const
{
    return _isBatch;
}

void StartupParameters::setIsBatch(bool isBatch)
{
    _isBatch = isBatch;
}

QString StartupParameters::file1() const
{
    return _file1;
}

void StartupParameters::setFile1(const QString &file1)
{
    _file1 = file1;
}

QString StartupParameters::file2() const
{
    return _file2;
}

void StartupParameters::setFile2(const QString &file2)
{
    _file2 = file2;
}

int StartupParameters::pages() const
{
    return _pages;
}

void StartupParameters::setPages(int pages)
{
    _pages = pages;
}

int StartupParameters::startPage1() const
{
    return _startPage1;
}

void StartupParameters::setStartPage1(int startPage1)
{
    _startPage1 = startPage1;
}

int StartupParameters::startPage2() const
{
    return _startPage2;
}

void StartupParameters::setStartPage2(int startPage2)
{
    _startPage2 = startPage2;
}

bool StartupParameters::enablePDFDiff() const
{
    return _enablePDFDiff;
}

void StartupParameters::setEnablePDFDiff(bool enablePDFDiff)
{
    _enablePDFDiff = enablePDFDiff;
}

QString StartupParameters::pdfDiffFilePath() const
{
    return _pdfDiffFilePath;
}

void StartupParameters::setPdfDiffFilePath(const QString &pdfDiffFilePath)
{
    _pdfDiffFilePath = pdfDiffFilePath;
}

InitialComparisonMode StartupParameters::comparisonMode() const
{
    return _comparisonMode;
}

void StartupParameters::setComparisonMode(const InitialComparisonMode &comparisonMode)
{
    _comparisonMode = comparisonMode;
}

bool StartupParameters::useXMLResultFile() const
{
    return _useXmlResult;
}

QString StartupParameters::XMLResultFilePath() const
{
    return _xmlResultFile;
}

QString StartupParameters::key() const
{
    return _key;
}

QDateTime StartupParameters::startTime() const
{
    return _startTime;
}

void StartupParameters::setStartTime(const QDateTime &startTime)
{
    _startTime = startTime;
}

QString StartupParameters::settingsFile() const
{
    return _settingsFile;
}

void StartupParameters::setSettingsFile(const QString &settingsFile)
{
    _settingsFile = settingsFile;
}
