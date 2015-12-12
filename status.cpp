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


#include "status.h"
#include <QTextStream>

Status::Status()
{
    _status = ENoErrors ;
}

Status::~Status()
{
}

bool Status::isError()
{
    if( ENoErrors == _status ) {
        return false;
    }
    return true ;
}

bool Status::isErrorComparing()
{
    switch(_status) {
    default:
        return false;
    case ErrorDocDiffer:
    case ErrorPagesDiffer:
        return true ;
    }
}

EError Status::status() const
{
    return _status;
}

void Status::setStatus(const EError &status)
{
    _status = status;
}

void Status::setStatusWithDescription(const EError &status, const QString newDescription)
{
    if( ENoErrors == _status ) {
        setStatusWithDescriptionUncond(status, newDescription);
    }
}

void Status::setStatusWithDescriptionUncond(const EError &status, const QString newDescription)
{
    _status = status ;
    _description = newDescription ;
}

void Status::setParamError(const bool error, const QString &paramName)
{
    if( error ) {
        setStatusWithDescriptionUncond(ErrorParameters, "");
        _paramInError = paramName ;
    }
}

int Status::returnOp(const EReturnType retType)
{
    QString description ;
    if( _description.isEmpty() ) {
        switch(_status) {
        case ENoErrors:
            description = QObject::tr("documents are the same");
            break;
        case ErrorParameters:
            description = QObject::tr("command line parameter error :%1").arg(_paramInError);
            break;
        case ErrorUnspecified:
        default:
            description = QObject::tr("unexpected error");
            break;
        case ErrorDocDiffer:
            description = QObject::tr("documents differ");
            break;
        case ErrorPagesDiffer:
            description = QObject::tr("the number of pages is not the same on both the documents");
            break;
        case ErrorUnableToLoadFile1:
            description = QObject::tr("unable to load file 1");
            break;
        case ErrorUnableToLoadFile2:
            description = QObject::tr("unable to load file 2");
            break;
        }
    } else {
        description = _description;
    }
    int retCode = static_cast<int>(_status);
    QTextStream out(stdout);
    switch( retType ) {
    default:
    case ReturnCodeAndDescritpion:
        out << retCode << "|" << description << "\n";
        break;
    case ReturnOnlyCode:
        out << retCode << "\n";
        break;
    }
    return retCode;
}
