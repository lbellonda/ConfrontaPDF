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
#include "batchcompare.h"
#include "aboutform.hpp"
#include <QTextStream>
#include <QFile>
#include <QDateTime>

Status::Status()
{
    _pagesNotEqualCount = 0 ;
    _status = ENoErrors ;
    _doc1Info = NULL ;
    _doc2Info = NULL ;
}

Status::~Status()
{
    setDoc1Info(NULL);
    setDoc2Info(NULL);
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

DocInfo *Status::doc1Info() const
{
    return _doc1Info;
}

void Status::setDoc1Info(DocInfo *doc1Info)
{
    if( NULL != _doc1Info) {
        delete _doc1Info ;
    }
    _doc1Info = doc1Info;
}

DocInfo *Status::doc2Info() const
{
    return _doc2Info;
}

void Status::setDoc2Info(DocInfo *doc2Info)
{
    if( NULL != _doc2Info) {
        delete _doc2Info ;
    }
    _doc2Info = doc2Info;
}


int Status::returnOp(const EReturnType retType, StartupParameters *params, BatchCompare *compare)
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

    if( params->useXMLResultFile() ) {
        QDateTime now = QDateTime::currentDateTime();
        bool okFile = false;
        QFile outFile(params->XMLResultFilePath());
        if( outFile.open(QFile::WriteOnly|QFile::Truncate) ) {
            QXmlStreamWriter stream(&outFile);
            stream.setAutoFormatting(true);
            stream.writeStartDocument();
            stream.writeStartElement("diffResult");
            stream.writeAttribute("code", QString::number(retCode) );
            stream.writeAttribute("description", description);
            stream.writeAttribute("date", now.toString(Qt::ISODate) );
            stream.writeAttribute("key", params->key() );
            //----
            stream.writeStartElement("args");
            stream.writeAttribute("mode", QString::number(params->comparisonMode()));
            stream.writeAttribute("file1", params->file1());
            stream.writeAttribute("file2", params->file2());
            stream.writeAttribute("pages", QString::number(params->pages()));
            stream.writeAttribute("startPage1", QString::number(params->startPage1()));
            stream.writeAttribute("startPage2", QString::number(params->startPage2()));
            stream.writeAttribute("pageDiffCount", QString::number(_pagesNotEqualCount));
            stream.writeAttribute("settings", params->settingsFile());
            stream.writeEndElement(); // args
            //-----
            stream.writeStartElement("info");
            if( NULL != _doc1Info ) {
                stream.writeStartElement("info1");
                // write info 1
                writeInfo(stream, _doc1Info);
                stream.writeEndElement(); // info1
            }
            if( NULL != _doc2Info ) {
                stream.writeStartElement("info2");
                // write info 2
                writeInfo(stream, _doc2Info);
                stream.writeEndElement(); // info2
            }
            stream.writeEndElement(); // info

            //---
            stream.writeStartElement("run");
            qint64 msecs = params->startTime().msecsTo(now);
            stream.writeAttribute("durationInSec", QString::number((((double)msecs)/1000.0)));
            stream.writeAttribute("swVersion", AboutForm::Version);
            stream.writeAttribute("xmlResultFile", params->XMLResultFilePath());
            stream.writeAttribute("pdfDiffFile", params->pdfDiffFilePath());
            stream.writeEndElement(); // run

            if( NULL != compare ) {
                stream.writeStartElement("parameters");
                compare->writeParameters(stream);
                stream.writeEndElement(); // parameters
            }

            stream.writeEndElement(); // diffResult
            stream.writeEndDocument();
            outFile.flush();
            outFile.close();
            if( outFile.error() == QFile::NoError ) {
                okFile = true ;
            }
            //fare per info nell'xml risutlato
            //fare parametri di lancio, tempo start e tempo end, durata e descrizione dei pdf
        }
        if( !okFile ) {
            QTextStream out(stdout);
            out << ErrorWritingXMLResultFile << "|" << QObject::tr("Error writing XML result file:%1").arg(outFile.errorString()) << "\n";
            return ErrorWritingXMLResultFile ;
        }
    }
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
    out.flush();
    return retCode;
}

//----
void Status::writeInfo(QXmlStreamWriter &writer, DocInfo *info)
{
    writer.writeAttribute("filename", info->fileName);
    writer.writeAttribute("pageCount", QString::number(info->pageCount));
    writer.writeAttribute("creationDate", info->creationDate);
    writer.writeAttribute("modDate", info->modDate);
    writer.writeAttribute("pageSize", info->pageSize);
    writer.writeStartElement("items");
    int size = info->infos.size();
    for( int i = 0 ; i < size ; i ++ ) {
        const QPair<QString,QString> &item = info->infos.at(i);
        writer.writeStartElement("item");
        writer.writeAttribute("name", item.first);
        writer.writeAttribute("value", item.second);
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeStartElement("fonts");
    foreach( LFontInfo *fi, info->fonts ) {
        writer.writeStartElement("font");
        writer.writeAttribute("name", fi->name);
        writer.writeAttribute("type", fi->typeName);
        writer.writeAttribute("embedded", fi->embedded?"true":"false");
        writer.writeAttribute("subset", fi->subset?"true":"false");
        writer.writeEndElement();
    }

    writer.writeEndElement();
}

//-----

int Status::pagesNotEqualCount() const
{
    return _pagesNotEqualCount;
}

void Status::setPagesNotEqualCount(int pagesNotEqualCount)
{
    _pagesNotEqualCount = pagesNotEqualCount;
}
