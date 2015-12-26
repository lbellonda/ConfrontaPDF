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


#ifndef STATUS_H
#define STATUS_H

#include "docinfo.h"
#include "startupparameters.h"
#include <QXmlStreamWriter>

enum EError {
    ENoErrors = 0,
    //
    ErrorParameters=-1,
    ErrorUnspecified=-2,
    ErrorUnableToLoadFile1=-3,
    ErrorUnableToLoadFile2=-4,
    ErrorLoadingPage=-5,
    ErrorWritingPDFDiffFile=-6,
    ErrorWritingXMLResultFile=-7,
    ErrorUnableToLoadFile=-8,
    //
    ErrorDocDiffer=1,
    ErrorPagesDiffer=2,
    ErrorInitialPage=3,
    ErrorFinalPage=4,
    ErrorSameFile=5,
};

enum EReturnType;

class BatchCompare ;

class Status
{
    EError _status;
    QString _description;
    QString _paramInError;
    DocInfo *_doc1Info;
    DocInfo *_doc2Info;
public:
    Status();
    ~Status();

    int returnOp(const EReturnType retType, StartupParameters *params, BatchCompare *compare = NULL);
    void setParamError(const bool error, const QString &paramName);

    EError status() const;
    void setStatus(const EError &status);
    void setStatusWithDescription(const EError &status, const QString newDescription);
    void setStatusWithDescriptionUncond(const EError &status, const QString newDescription);

    bool isError();
    bool isErrorComparing();
    DocInfo *doc1Info() const;
    void setDoc1Info(DocInfo *doc1Info);
    DocInfo *doc2Info() const;
    void setDoc2Info(DocInfo *doc2Info);

    private:
    void writeInfo(QXmlStreamWriter &writer, DocInfo *info);
};

#endif // STATUS_H
